#include "daScene_Title.h"

#include "daResources.h"
#include "daGameManager.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daBackGroundObject.h"
#include "daBackGroundScript.h"
#include "daOverlayObject.h"
#include "daUIObject.h"


// юс╫ц
#include "daInput.h"


namespace da
{
	Scene_Title::Scene_Title()
	{
	}
	Scene_Title::~Scene_Title()
	{
	}
	void Scene_Title::Initialize()
	{
		AddBackgroundObject();

		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->AddComponent<CameraScript>();
			cameraObj->Initialize();
			AddGameObject(enums::eLayerType::None, cameraObj);
			Camera* camera = cameraObj->GetCameraComponent();
			camera->TurnLayerMask(enums::eLayerType::UI, false);
		}
		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->Initialize();
			AddGameObject(enums::eLayerType::None, cameraObj);
			Camera* camera = cameraObj->GetCameraComponent();
			camera->DisableLayerMask();
			camera->TurnLayerMask(enums::eLayerType::UI);
		}
	}
	void Scene_Title::Update()
	{
		if (Input::GetKeyDown(da::eKeyCode::N))
		{
			GameManager::TurnOverlay(true);
		}
		Scene::Update();
	}
	void Scene_Title::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Title::Render()
	{
		Scene::Render();
	}
	void Scene_Title::AddBackgroundObject()
	{
		//BG
		{
			BackGroundObject* backGround = new BackGroundObject();
			backGround->Initialize();
			AddGameObject(enums::eLayerType::BackGround, backGround);
			MeshRenderer* backGroundRenderer = backGround->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleBackgroundMaterial"));
			BackGroundScript* bgScript = backGround->GetBGScript();
			bgScript->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			bgScript->TurnMovingObject(false);
		}
		// Back Cloud Layer
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"BackCloudMaterial"));
			BackGroundScript* bgScript = cloudObj->GetBGScript();
			bgScript->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			bgScript->SetPosition(math::Vector3(0.0f, 0.0f, -0.001f));
			bgScript->TurnMovingObject(false);
		}
		// Front Cloud Lyaer
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"FrontCloudMaterial"));
			BackGroundScript* bgScript = cloudObj->GetBGScript();
			bgScript->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			bgScript->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
			bgScript->TurnMovingObject(false);
		}
		// BGObj MainLogo
		{
			BackGroundObject* titleLogoObj = new BackGroundObject();
			titleLogoObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, titleLogoObj);
			MeshRenderer* backGroundRenderer = titleLogoObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"MainLogoMaterial"));
			BackGroundScript* bgScript = titleLogoObj->GetBGScript();
			bgScript->SetScale(math::Vector3(4.680f, 2.250f, 1.0f));
			bgScript->SetPosition(math::Vector3(0.0f, 2.250f, -0.0030f));
			bgScript->TurnMovingObject(false);
		}
	}
	void Scene_Title::OnEnter()
	{
	}
	void Scene_Title::OnExit()
	{
	}
}
