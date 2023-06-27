#include "daTitleScene.h"

#include "daResources.h"
#include "daGameManager.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"

#include "daBackGroundObject.h"
#include "daBackGroundScript.h"
#include "daOverlayObject.h"
#include "daUIObject.h"
// �ӽ�
#include "daInput.h"


namespace da
{
	TitleScene::TitleScene()
	{
	}
	TitleScene::~TitleScene()
	{
	}
	void TitleScene::Initialize()
	{
		AddBackgroundObject();

		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->Initialize();
			AddGameObject(enums::eLayerType::None, cameraObj);
		}
	}
	void TitleScene::Update()
	{
		if (Input::GetKeyDown(da::eKeyCode::N))
		{
			GameManager::TurnOverlay(true);
		}
		Scene::Update();
	}
	void TitleScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void TitleScene::Render()
	{
		Scene::Render();
	}
	void TitleScene::AddBackgroundObject()
	{
		//BG
		{
			BackGroundObject* backGround = new BackGroundObject();
			backGround->Initialize();
			AddGameObject(enums::eLayerType::BackGround, backGround);
			MeshRenderer* backGroundRenderer = backGround->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleBackGroundMaterial"));
			BackGroundScript* bgScript = backGround->GetBGScript();
			bgScript->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			bgScript->TurnMovingObject(false);
		}
		// BGObj cloud
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudAMaterial"));
			BackGroundScript* bgScript = cloudObj->GetBGScript();
			bgScript->SetScale(math::Vector3(2.0f, 1.44f, 1.0f));
			bgScript->SetPosition(math::Vector3(-4.0f, 1.0f, -1.0f));
		}
		// BGObj cloud
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudBMaterial"));
			BackGroundScript* bgScript = cloudObj->GetBGScript();
			bgScript->SetScale(math::Vector3(2.50f, 1.51f, 1.0f));
			bgScript->SetPosition(math::Vector3(3.0f, 1.0f, -1.0f));
		}
		// BGObj cloud
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, cloudObj);
			MeshRenderer* backGroundRenderer = cloudObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CloudCMaterial"));
			BackGroundScript* bgScript = cloudObj->GetBGScript();

			bgScript->SetScale(math::Vector3(6.0f, 3.81f, 1.0f));
			bgScript->SetPosition(math::Vector3(2.50f, -2.0f, -1.0f));
		}

		// BGObj titleLogo
		{
			BackGroundObject* titleLogoObj = new BackGroundObject();
			titleLogoObj->Initialize();
			titleLogoObj->SetDisappearObject(true);
			AddGameObject(enums::eLayerType::BackGround, titleLogoObj);
			MeshRenderer* backGroundRenderer = titleLogoObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleLogoMaterial"));
			BackGroundScript* bgScript = titleLogoObj->GetBGScript();
			bgScript->SetScale(math::Vector3(6.0f, 1.70f, 1.0f));
			bgScript->SetPosition(math::Vector3(0.0f, 2.0f, -1.10f));
		}

		// Button Object
		{
			UIObject* goToSelectButton = new UIObject();
			goToSelectButton->Initialize();
			goToSelectButton->SetDisappearObject(true);
			AddGameObject(enums::eLayerType::UI, goToSelectButton);
			MeshRenderer* buttonRenderer = goToSelectButton->AddComponent<MeshRenderer>();
			buttonRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			buttonRenderer->SetMaterial(Resources::Find<Material>(L"GotoSelectButtonMaterial"));
			Transform* buttonTr = goToSelectButton->GetComponent<Transform>();

			buttonTr->SetScale(math::Vector3(4.20f, 0.60f, 1.0f));
			buttonTr->SetPosition(math::Vector3(0.0f, 0.50, -1.20f));
		}
	}
}
