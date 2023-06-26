#include "daPlayScene.h"

#include "daResources.h"

#include "daGameObject.h"

#include "daTransform.h"
#include "daMesh.h"
#include "daMeshRenderer.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daBackGroundObject.h"
#include "daBackGroundScript.h"

namespace da
{
	using namespace enums;

	PlayScene::PlayScene()
	{
	}
	PlayScene::~PlayScene()
	{
	}
	void PlayScene::Initialize()
	{
		srand((unsigned int)time(0));
		InitBGObj();

		{
			/*GameObject* player = new GameObject();
			AddGameObject(eLayerType::Playable, player);
			MeshRenderer* playerRenderer = player->AddComponent<MeshRenderer>();
			playerRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			playerRenderer->SetMaterial(Resources::Find<Material>(L"SpriteMaterial"));
			Transform* playerTr = player->GetComponent<Transform>();
			playerTr->SetPosition(math::Vector3(0.5f, 0.5f, 0.0f));*/
		}

		{
			/*GameObject* cameraObj = new GameObject();
			AddGameObject(eLayerType::None, cameraObj);
			cameraObj->GetComponent<Transform>()->SetPosition(math::Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = cameraObj->AddComponent<Camera>();
			cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			CameraScript* cameraScript = cameraObj->AddComponent<CameraScript>();*/
		}

		{
			CameraObject* cameraObj = new CameraObject();
			cameraObj->Initialize();
			AddGameObject(eLayerType::None, cameraObj);
		}
	}
	void PlayScene::Update()
	{
		Scene::Update();
	}
	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void PlayScene::Render()
	{
		Scene::Render();
	}

	void PlayScene::InitBGObj()
	{
		//BG
		{
			BackGroundObject* backGround = new BackGroundObject();
			backGround->Initialize();
			AddGameObject(enums::eLayerType::a, backGround);
			MeshRenderer* backGroundRenderer = backGround->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleBackGroundMaterial"));
			BackGroundScript* bgScript = backGround->GetBGScript();
			bgScript->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			bgScript->SetMovingObject(false);
		}

		// BGObj cloud
		{
			BackGroundObject* cloudObj = new BackGroundObject();
			cloudObj->Initialize();
			AddGameObject(enums::eLayerType::b, cloudObj);
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
			AddGameObject(enums::eLayerType::c, cloudObj);
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
			AddGameObject(enums::eLayerType::d, cloudObj);
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
			AddGameObject(enums::eLayerType::e, titleLogoObj);
			MeshRenderer* backGroundRenderer = titleLogoObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"TitleLogoMaterial"));
			BackGroundScript* bgScript = titleLogoObj->GetBGScript();
			bgScript->SetScale(math::Vector3(6.0f, 1.70f, 1.0f));
			bgScript->SetPosition(math::Vector3(0.0f, 2.0f, -1.10f));
		}
		{
			GameObject* goToSelectButton = new GameObject();
			goToSelectButton->Initialize();
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
