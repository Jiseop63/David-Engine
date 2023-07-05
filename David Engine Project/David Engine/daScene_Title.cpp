#include "daScene_Title.h"

// resource
#include "daResources.h"
#include "daTexture.h"
#include "daRenderer.h"

// objects & components
#include "daObjects.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daTimeConstants.h"

#include "daUIObject.h"
#include "daButtonScript.h"

#include "daCursorScript.h"

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
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		renderer::mainCamera = mainCameraObj->GetCameraComponent();
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		renderer::uiCamera = uiCameraObj->GetCameraComponent();

		addBackgroundObjects();
		addUIObjects();

	}
	void Scene_Title::Update()
	{
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

	void Scene_Title::OnEnter()
	{
	}
	void Scene_Title::OnExit()
	{
	}


	void Scene_Title::addBackgroundObjects()
	{
		// just BG
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"TitleBackgroundMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
		}

		// moving background layer
		{
			// Back & Front Cloud Layer
			GameObject* cloudObj = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"BackCloudMaterial");
			cloudObj->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			cloudObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.001f));
			TimeConstants* cloudTimer = cloudObj->AddComponent<TimeConstants>();
			cloudTimer->SetValue(0.1f);


			GameObject* cloudObj2 = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"FrontCloudMaterial");
			cloudObj2->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			cloudObj2->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
			TimeConstants* cloudTimer2 = cloudObj2->AddComponent<TimeConstants>();
			cloudTimer2->SetValue(0.7f);

		}
		// BGObj MainLogo
		{
			GameObject* titleLogoObj = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"MainLogoMaterial");
			titleLogoObj->GetTransform()->SetScale(math::Vector3(4.680f, 2.250f, 1.0f));
			titleLogoObj->GetTransform()->SetPosition(math::Vector3(0.0f, 1.750f, -0.0030f));
		}
		// BGObj Copyright
		{
			GameObject* copyrightObj = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"CopyrightMaterial");
			copyrightObj->GetTransform()->SetScale(math::Vector3(5.010f, 0.360f, 1.0f));
			copyrightObj->GetTransform()->SetPosition(math::Vector3(0.0f, -2.750f, -0.0030f));
		}
	}

	void Scene_Title::addUIObjects()
	{
		// mouse
		{
			GameObject* cursorObject = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"BasicCursorMaterial");
			cursorObject->GetTransform()->SetScale(math::Vector3(0.190f * 4.0f, 0.190f * 4.0f, 1.0f));
			cursorObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.0050f));
			cursorObject->AddComponent<CursorScript>();
		}
		
		// buttons
		{
			UIObject* playBtnObject = objects::InstantiateButtonObject<UIObject>(this, L"StartBtnMaterial", L"PlayOffTexture", L"PlayOnTexture");
			playBtnObject->GetTransform()->SetScale(math::Vector3(1.440f, 0.480f, 1.0f));
			playBtnObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.003f));
			ButtonScript* playBtnScript = playBtnObject->GetComponent<ButtonScript>();
			playBtnScript->SetScreenPosision();
			playBtnScript->SetButtonType(ButtonScript::eButtonType::Play);
			UIObject* exitBtnObject = objects::InstantiateButtonObject<UIObject>(this, L"ExitBtnMaterial", L"ExitOffTexture", L"ExitOnTexture");
			exitBtnObject->GetTransform()->SetScale(math::Vector3(0.840f, 0.480f, 1.0f));
			exitBtnObject->GetTransform()->SetPosition(math::Vector3(0.0f, -1.0f, -0.003f));
			ButtonScript* exitBtnScript = exitBtnObject->GetComponent<ButtonScript>();
			exitBtnScript->SetScreenPosision();
			exitBtnScript->SetButtonType(ButtonScript::eButtonType::Exit);
		}
	}
}
