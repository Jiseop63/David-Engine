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

		// 씬의 오브젝트들을 렌더모드에 따라 렌더링배열에 추가
		alphaSortGameObjects();
		// 오브젝트들을 z축으로 정렬
		depthSortTransparencyGameobjects();
		// 불투명 객체부터 우선 렌더
		renderOpaque();
		// depthState 설정 끄기
		disableDepthStencilState();
		// 일부 투명 객체 렌더
		renderCutout();
		// 반투명 객체 렌더
		renderTransparent();
		// depthState 설정 켜기
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
		// 각 렌더 오브젝트들 초기화
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();

		// 렌더할 씬으로부터 오브젝트 배열 가져오기
		Scene* scene = SceneManager::GetActiveScene();
		
				
		for (int layerType = 0; layerType < (UINT)enums::eLayerType::End; ++layerType)
		{
			if (true == mLayerMask[layerType])
			{
				// 씬에서 레이어에 있는 오브젝트 가져옴
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
			// 렌더러 컴포넌트를 가져와서 렌더모드를 확인함
			MeshRenderer* meshRenderer = object->GetComponent<MeshRenderer>();
			if (nullptr == meshRenderer)
				continue;

			std::shared_ptr<Material> material = meshRenderer->GetMaterial();
			eRenderingMode renderMode = material->GetRenderingMode();

			// 타입에 맞게 정렬해줌
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
