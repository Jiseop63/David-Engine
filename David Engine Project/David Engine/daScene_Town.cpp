#include "daScene_Town.h"

#include "daRenderer.h"
// 임시
#include "daInput.h"
#include "daSceneManager.h"
#include "daApplication.h"

// resource
#include "daResources.h"
#include "daTexture.h"

#include "daObjects.h"

// UI, Object, Components and Camera
#include "daObjectsFastIncludeHeader.h"

extern da::Application application;

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
		mMainCamera = objects::InstantiateMainCamera(this);
		mUICamera = objects::InstantiateUICamera(this);

		objects::InstantiateGridObject(this, mMainCamera);		
		addBackgroundObject();
		addUIObjects();
		addGameObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			//SceneManager::LoadScene(L"Scene_Dungeon1F");
			int a = 0;
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
		// 카메라 세팅
		renderer::mainCamera = mMainCamera->GetCameraComponent();
		renderer::uiCamera = mUICamera->GetCameraComponent();
		// 플레이어 세팅
	}
	void Scene_Town::OnExit()
	{
	}

	void Scene_Town::addBackgroundObject()
	{
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::BackGround, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
			backGround->SetName(L"sky");
		}

		// 레이어 역할을 하는 배경들은 스케일 값을 건드는게 아니라 카메라로부터 영향받는 수치를 낮추는 식으로 해야할듯?
		// town BG : follow
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -3.0f, BackLayerZ));
			backGround->SetName(L"forest");
		}

		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -6.0f, FrontLayerZ));
			backGround->SetName(L"trees");
		}
	}
	void Scene_Town::addUIObjects()
	{
#pragma region HUD
		// player life panel, dash panel
		{
			// HUD 객체 생성
			GameObject* lifeHUD = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::None);
			lifeHUD->SetName(L"lifeHUD");
			Transform* lifeHUDTransform = lifeHUD->GetTransform();
			//  HUD 위치 조절 (좌상단)
			lifeHUDTransform->SetPosition(Vector3(-MaxPositionX, MaxPositionY, HUDZ));
			float padding = 0.20f;

			// Panel 객체 생성
			GameObject* lifePanel = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
			lifePanel->SetName(L"lifePanel");
			Transform* lifePanelTransform = lifePanel->GetTransform();
			lifePanelTransform->SetParent(lifeHUDTransform);
			// Panel 크기 조절
			float panelXScale = 2.960f;	// 74 * 4
			float panelYScale = 0.640f;	// 16 * 4
			lifePanelTransform->SetScale(Vector3(panelXScale, panelYScale, 1.0f));
			// Panel 위치 조절
			Vector3 lifePanelPosition = Vector3( (panelXScale / 2.0f) + padding, -(panelYScale / 2.0f) - padding, 0.f);
			lifePanelTransform->SetPosition(lifePanelPosition);

			// Bar 객체 생성
			GameObject* lifeBar = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
			lifeBar->SetName(L"lifeBar");
			Transform* lifeBarTransform = lifeBar->GetTransform();
			lifeBarTransform->SetParent(lifeHUDTransform);
			// Bar 크기 조절
			float barXScale = 1.960f;	// 49 * 4
			float barYScale = 0.40f;	// 10 * 4
			lifeBarTransform->SetScale(Vector3(barXScale, barYScale, 1.0f));
			// Bar 위치 조절하기
			// 22 * 4
			// hud 위치에서 panel size의 절반만큼 당긴다음에... bar offset만큼 더 당기면될듯?
			Vector3 lifeBarPosition = lifePanelPosition + Vector3(0.380f, 0.0f, -0.0001f);
			lifeBarTransform->SetPosition(lifeBarPosition);

			lifeBar->AddComponent<LifeBarScript>();
			//GameObject* dashPanel = objects::InstantiateGameObject
			//	<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");

			//Transform* dashPanelTransform = dashPanel->GetTransform();


			//float dashXScale = 0.880f;
			//float dashYScale = 0.640f;

			//dashPanelTransform->SetScale(Vector3(dashXScale, dashYScale, 1.0f));
			//float dashWidthScaleHalf = dashXScale / 2.0f;
			//float dashHeightScaleHalf = dashYScale / 2.0f;
			//float dashPanePadding = 0.050f;
			//Vector3 dashPanelPosition = Vector3(-MaxPositionX + dashWidthScaleHalf + padding, MaxPositionY - panelYScale - dashHeightScaleHalf - padding, HUDZ);
			//dashPanelTransform->SetPosition(dashPanelPosition);
		}
				
		// player weapon panel
		{
			GameObject* weaponPanel = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			weaponPanel->SetName(L"weaponPanel");
			Transform* weaponPanelTransform = weaponPanel->GetTransform();
			float weaponPanelScaleX = 1.70f;
			float weaponPanelScaleY = 1.0f;
			weaponPanelTransform ->SetScale(math::Vector3(weaponPanelScaleX, weaponPanelScaleY, 1.0f));
			float dashPanePadding = 0.150f;

			Vector3 weaponPanelPosition 
				= Vector3(
					MaxPositionX - (weaponPanelScaleX / 2.0f) - dashPanePadding
					, -MaxPositionY + (weaponPanelScaleY / 2.0f) + dashPanePadding, HUDZ);
			weaponPanelTransform->SetPosition(weaponPanelPosition);
		}
		// mouse
		{
			GameObject* cursorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"ShootingCursorMaterial");
			cursorObject->SetName(L"cursor");
			cursorObject->GetTransform()->SetScale(math::Vector3(0.210f * 4.0f, 0.210f * 4.0f, 1.0f));
			cursorObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CursorZ));
			cursorObject->AddComponent<CursorScript>();
		}
#pragma endregion
#pragma region Inventory
		// Inventory Base
		{
			float inventoryScaleX = 1.230f;
			float inventoryScaleY = 1.80f;
			GameObject* inventoryObject = objects::InstantiateGameObject<GameObject>
				(this, enums::eLayerType::UI, L"InventoryBaseMaterial");
			inventoryObject->SetName(L"inventory");
			inventoryObject->GetTransform()->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			inventoryObject->GetTransform()->SetPosition(math::Vector3(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ));
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();
			inventoryObject->SetObjectState(GameObject::eObjectState::Hide);
		}

#pragma endregion
	}
	void Scene_Town::addGameObjects()
	{
		// sky BG : stay
		{
			GameObject* playerObject = objects::InstantiatePlayer(this, L"SampleMaterial");
			playerObject->SetName(L"player");
		}
	}
}
