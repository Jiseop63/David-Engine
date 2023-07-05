#include "daScene_Town.h"

// 임시
#include "daInput.h"
#include "daSceneManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"

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


namespace da
{
	Scene_Town::Scene_Town()
	{
	}
	Scene_Town::~Scene_Town()
	{
	}
	void Scene_Town::Initialize()
	{
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		addBackgroundObject();
		addUIObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F");
		}
		Scene::Update();
	}
	void Scene_Town::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Town::Render()
	{
		Scene::Render();
	}
	
	void Scene_Town::OnEnter()
	{
		// 카메라 세팅, 플레이어 세팅
	}
	void Scene_Town::OnExit()
	{
	}

	void Scene_Town::addBackgroundObject()
	{
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
		}

		// 레이어 역할을 하는 배경들은 스케일 값을 건드는게 아니라 카메라로부터 영향받는 수치를 낮추는 식으로 해야할듯?
		// town BG : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -3.0f, -0.001f));
		}

		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -6.0f, -0.002f));
		}
	}
	void Scene_Town::addUIObjects()
	{
		// player life panel, dash panel
		{
			GameObject* lifePanel = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");

			Transform* lifePanelTransform = lifePanel->GetTransform();


			float lifeXScale = 2.960f;
			float lifeYScale = 0.640f;



			lifePanelTransform->SetScale(Vector3(lifeXScale, lifeYScale, 1.0f));
			float lifeWidthScaleHalf = lifeXScale / 2.0f;
			float lifeHeightScaleHalf = lifeYScale / 2.0f;
			float lifePanelPadding = 0.20f;
			Vector3 lifePanelPosition = Vector3(-6.830f + lifeWidthScaleHalf + lifePanelPadding, 3.840f - lifeHeightScaleHalf - lifePanelPadding, -0.0050f);
			lifePanelTransform->SetPosition(lifePanelPosition);


			GameObject* dashPanel = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");

			Transform* dashPanelTransform = dashPanel->GetTransform();


			float dashXScale = 0.880f;
			float dashYScale = 0.640f;

			dashPanelTransform->SetScale(Vector3(dashXScale, dashYScale, 1.0f));
			float dashWidthScaleHalf = dashXScale / 2.0f;
			float dashHeightScaleHalf = dashYScale / 2.0f;
			float dashPanePadding = 0.050f;
			Vector3 dashPanelPosition = Vector3(-6.830f + dashWidthScaleHalf + lifePanelPadding, 3.840f - (lifeHeightScaleHalf * 2) - dashHeightScaleHalf - lifePanelPadding, -0.0050f);
			dashPanelTransform->SetPosition(dashPanelPosition);
		}
		// player dash panel
		{


		}
		// player weapon panel
		{
			GameObject* weaponBase = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			weaponBase->GetTransform()->SetScale(math::Vector3(1.70f, 1.20f, 1.0f));
			weaponBase->GetTransform()->SetPosition(math::Vector3(4.5f, -2.5f, -0.005f));
		}
	}
}
