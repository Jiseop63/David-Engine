#include "daCamera.h"
#include "daApplication.h"
#include "daRenderer.h"
#include "daSceneManager.h"

#include "daLayer.h"
#include "daGameObject.h"

#include "daTransform.h"
#include "daMeshRenderer.h"

extern da::Application application;
namespace da
{
	using namespace math;

	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	bool CompareDepthSort(GameObject* left, GameObject* right)
	{
		if (left->GetComponent<Transform>()->GetPosition().z
			<= right->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Camera::Camera()
		: Component(enums::eComponentType::Camera)
		, mProjectionType(eProjectionType::Orthographic)
		, mTransform(nullptr)
		, mView(Matrix::Identity)
		, mProjection(Matrix::Identity)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mSize(1.0f)
		, mLayerMask{}
		, mOpaqueGameObjects{}
		, mCutoutGameObjects{}
		, mTransparentGameObjects{}
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		EnableLayerMask();
	}
	void Camera::LateUpdate()
	{
		createViewMatrix();
		createProjectionMatrix();
		registerCameraInRanderer();
	}
	void Camera::Render()
	{
		View = mView;
		Projection = mProjection;

		// ���� ������Ʈ���� ������忡 ���� �������迭�� �߰�
		alphaSortGameObjects();
		// ������Ʈ���� z������ ����
		depthSortTransparencyGameobjects();
		// ������ ��ü���� �켱 ����
		renderOpaque();
		// depthState ���� ����
		disableDepthStencilState();
		// �Ϻ� ���� ��ü ����
		renderCutout();
		// ������ ��ü ����
		renderTransparent();
		// depthState ���� �ѱ�
		enableDepthStencilState();
		
	}

	void Camera::TurnLayerMask(enums::eLayerType layerType, bool enable)
	{
		mLayerMask.set( (UINT)layerType, enable );
	}


	bool Camera::createViewMatrix()
	{
		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector3 pos = transform->GetPosition();

		// View Translate Matrix
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-pos);

		// View Rotation Matrix
		Vector3 up = transform->Up();
		Vector3 right = transform->Right();
		Vector3 foward = transform->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
		mView *= viewRotate;

		return true;
	}
	bool Camera::createProjectionMatrix()
	{		
		float width = (float)application.GetClientWidth();
		float height = (float)application.GetClientHeight();
		mAspectRatio = width / height;

		if (mProjectionType == eProjectionType::Orthographic)
		{
			float OrthorGraphicRatio = mSize / 100.0f;
			width *= OrthorGraphicRatio;
			height *= OrthorGraphicRatio;

			mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
		}
		else
		{
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
		}

		return true;
	}


	void Camera::registerCameraInRanderer()
	{
		renderer::cameras.push_back(this);
	}
	void Camera::alphaSortGameObjects()
	{
		// �� ���� ������Ʈ�� �ʱ�ȭ
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();

		// ������ �����κ��� ������Ʈ �迭 ��������
		Scene* scene = SceneManager::GetActiveScene();
		
				
		for (int layerType = 0; layerType < (UINT)enums::eLayerType::End; ++layerType)
		{
			if (true == mLayerMask[layerType])
			{
				// ������ ���̾ �ִ� ������Ʈ ������
				Layer& targetLayer = scene->GetLayer((da::enums::eLayerType)layerType);
				const std::vector<GameObject*> gameObjects = targetLayer.GetGameObjects();

				divideAlphaBlendObjects(gameObjects);
				
			}
		}
	}

	void Camera::depthSortTransparencyGameobjects()
	{
		std::sort(mCutoutGameObjects.begin()
			, mCutoutGameObjects.end()
			, CompareDepthSort);
		std::sort(mTransparentGameObjects.begin()
			, mTransparentGameObjects.end()
			, CompareDepthSort);
	}

	void Camera::divideAlphaBlendObjects(const std::vector<GameObject*> objects)
	{
		for (GameObject* object : objects)
		{
			// ������ ������Ʈ�� �����ͼ� ������带 Ȯ����
			MeshRenderer* meshRenderer = object->GetComponent<MeshRenderer>();
			if (nullptr == meshRenderer)
				continue;

			std::shared_ptr<Material> material = meshRenderer->GetMaterial();
			eRenderingMode renderMode = material->GetRenderingMode();

			// Ÿ�Կ� �°� ��������
			switch (renderMode)
			{
			case da::graphics::eRenderingMode::Opaque:
				mOpaqueGameObjects.push_back(object);
				break;
			case da::graphics::eRenderingMode::Cutout:
				mCutoutGameObjects.push_back(object);
				break;
			case da::graphics::eRenderingMode::Transparent:
				mTransparentGameObjects.push_back(object);
				break;
			}
		}
	}
	
	void Camera::renderOpaque()
	{
		for (GameObject* opaqueObject : mOpaqueGameObjects)
		{
			if (nullptr == opaqueObject)
				continue;
			if (opaqueObject->GetObjectState()
				== GameObject::eObjectState::Hide)
				continue;
			if (opaqueObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;

			opaqueObject->Render();
		}
	}
	void Camera::renderCutout()
	{
		for (GameObject* cutoutObject : mCutoutGameObjects)
		{
			if (nullptr == cutoutObject)
				continue;
			if (cutoutObject->GetObjectState()
				== GameObject::eObjectState::Hide)
				continue;
			if (cutoutObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;

			cutoutObject->Render();
		}
	}
	void Camera::renderTransparent()
	{
		for (GameObject* transparentObject : mTransparentGameObjects)
		{
			if (nullptr == transparentObject)
				continue;
			if (transparentObject->GetObjectState()
				== GameObject::eObjectState::Hide)
				continue;
			if (transparentObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;

			transparentObject->Render();
		}
	}

	void Camera::enableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::DepthStencilStates[(UINT)eDSType::Less];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}
	void Camera::disableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::DepthStencilStates[(UINT)eDSType::None];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

}
