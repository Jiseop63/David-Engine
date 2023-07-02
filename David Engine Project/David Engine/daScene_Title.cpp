#include "daScene_Title.h"

#include "daResources.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daBackGroundObject.h"
#include "daBackGroundScript.h"
#include "daUIObject.h"


#include "daObjects.h"

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
			// Do something!!
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
			backGround->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
		}
		// Back & Front Cloud Layer		
		{
			BackGroundObject* cloudObj
				= objects::InstantiateObject
				<BackGroundObject>(this, enums::eLayerType::BackGround, L"BackCloudMaterial");			
			cloudObj->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			cloudObj->SetPosition(math::Vector3(0.0f, 0.0f, -0.001f));

			BackGroundScript* cloudScript = cloudObj->AddComponent<BackGroundScript>();
			cloudScript->SetSpeed(0.1f);

			BackGroundObject* cloudObj2
				= objects::InstantiateObject
				<BackGroundObject>(this, enums::eLayerType::BackGround, L"FrontCloudMaterial");
			cloudObj2->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			cloudObj2->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));

			BackGroundScript* cloudScript2 = cloudObj2->AddComponent<BackGroundScript>();
			cloudScript2->SetSpeed(0.8f);
			int a = 0;
		}
		{
			
		}
		// BGObj MainLogo
		{
			BackGroundObject* titleLogoObj = new BackGroundObject();
			titleLogoObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, titleLogoObj);
			MeshRenderer* backGroundRenderer = titleLogoObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"MainLogoMaterial"));
			titleLogoObj->SetScale(math::Vector3(4.680f, 2.250f, 1.0f));
			titleLogoObj->SetPosition(math::Vector3(0.0f, 1.750f, -0.0030f));
		}
		// BGObj Copyright
		{
			BackGroundObject* copyrightObj = new BackGroundObject();
			copyrightObj->Initialize();
			AddGameObject(enums::eLayerType::BackGround, copyrightObj);
			MeshRenderer* backGroundRenderer = copyrightObj->AddComponent<MeshRenderer>();
			backGroundRenderer->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			backGroundRenderer->SetMaterial(Resources::Find<Material>(L"CopyrightMaterial"));
			copyrightObj->SetScale(math::Vector3(5.010f, 0.360f, 1.0f));
			copyrightObj->SetPosition(math::Vector3(0.0f, -2.750f, -0.0030f));
		}
	}
	void Scene_Title::OnEnter()
	{
	}
	void Scene_Title::OnExit()
	{
	}
}
