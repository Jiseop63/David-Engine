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

	Camera::Camera()
		: Component(enums::eComponentType::Camera)
		, mProjectionType(eProjectionType::Perspective)
		, mTransform(nullptr)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mSize(10.0f)
		, mLayerMask{}
		, mOpaqueGameObjects{}
		, mCutoutGameObjects{}
		, mTransparentGameObjects{}
	{
		EnableLayerMask();
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}
	void Camera::Update()
	{
	}
	void Camera::LateUpdate()
	{
		createViewMatrix();
		createProjectionMatrix();
		registerCameraInRanderer();
	}
	void Camera::Render()
	{
		sortGameObjects();

		renderOpaque();
		renderCutout();
		renderTransparent();
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
		View = Matrix::Identity;
		View *= Matrix::CreateTranslation(-pos);

		// View Rotation Matrix
		Vector3 up = transform->Up();
		Vector3 right = transform->Right();
		Vector3 foward = transform->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
		viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
		viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
		View *= viewRotate;

		return true;
	}
	bool Camera::createProjectionMatrix()
	{
		RECT rect = {};
		GetClientRect(application.GetHwnd(), &rect);
		float width = float(rect.right - rect.left);
		float height = float(rect.bottom - rect.top);
		mAspectRatio = width / height;;



		if (mProjectionType == eProjectionType::Orthographic)
		{
			float OrthorGraphicRatio = mSize / 1000.0f;
			width *= OrthorGraphicRatio;
			height *= OrthorGraphicRatio;

			Projection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
		}
		else
		{
			Projection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
		}

		return true;
	}


	void Camera::registerCameraInRanderer()
	{
		renderer::cameras.push_back(this);
	}
	void Camera::sortGameObjects()
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

				
				for (GameObject* object : gameObjects)
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
					case da::graphics::eRenderingMode::CutOut:
						mCutoutGameObjects.push_back(object);
						break;
					case da::graphics::eRenderingMode::Transparent:
						mTransparentGameObjects.push_back(object);
						break;
					}
				}
			}
		}
	}
	
	void Camera::renderOpaque()
	{
		for (GameObject* opaqueObject : mOpaqueGameObjects)
		{
			if (nullptr == opaqueObject)
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

			cutoutObject->Render();
		}
	}
	void Camera::renderTransparent()
	{
		for (GameObject* transparentObject : mTransparentGameObjects)
		{
			if (nullptr == transparentObject)
				continue;

			transparentObject->Render();
		}
	}	
}
