#include "daScene_Dungeon1F.h"

#include "daRenderer.h"

// 임시
#include "daInput.h"
#include "daSceneManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"


// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"


namespace da
{
	Scene_Dungeon1F::Scene_Dungeon1F()
	{
	}
	Scene_Dungeon1F::~Scene_Dungeon1F()
	{
	}
	void Scene_Dungeon1F::Initialize()
	{
		mMainCamera = objects::InstantiateMainCamera(this);
		mUICamera = objects::InstantiateUICamera(this);

		addBackgroundObjects();
		addUIObjects();
	}
	void Scene_Dungeon1F::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon2F");
		}
		Scene::Update();
	}
	void Scene_Dungeon1F::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon1F::Render()
	{
		Scene::Render();
	}
	
	void Scene_Dungeon1F::OnEnter()
	{
		renderer::mainCamera = mMainCamera->GetCameraComponent();
		renderer::uiCamera = mUICamera->GetCameraComponent();
		renderer::gridScript->SetCamera(renderer::mainCamera);
	}
	void Scene_Dungeon1F::OnExit()
	{
	}

	void Scene_Dungeon1F::addBackgroundObjects()
	{
		// TempDungeonMaterial
		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TempDungeonMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
		}
	}
	void Scene_Dungeon1F::addUIObjects()
	{

#pragma region HUD
		// player life panel, dash panel
		{
			// HUD 객체 생성
			GameObject* lifeHUD = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::None);
			Transform* lifeHUDTransform = lifeHUD->GetTransform();
			//  HUD 위치 조절 (좌상단)
			lifeHUDTransform->SetPosition(Vector3(-MaxPositionX, MaxPositionY, HUDZ));
			float padding = 0.20f;
			// Panel 객체 생성
			GameObject* lifePanel = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
			Transform* lifePanelTransform = lifePanel->GetTransform();
			lifePanelTransform->SetParent(lifeHUDTransform);
			// Panel 크기 조절
			float panelXScale = 2.960f;	// 74 * 4
			float panelYScale = 0.640f;	// 16 * 4
			lifePanelTransform->SetScale(Vector3(panelXScale, panelYScale, 1.0f));
			// Panel 위치 조절
			Vector3 lifePanelPosition 
				= Vector3((panelXScale / 2.0f) + padding, -(panelYScale / 2.0f) - padding, 0.f);
			lifePanelTransform->SetPosition(lifePanelPosition);
			// Bar 객체 생성
			GameObject* lifeBar = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
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
			GameObject* weaponBase = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			Transform* weaponPanelTransform = weaponBase->GetTransform();
			float weaponPanelScaleX = 1.70f;
			float weaponPanelScaleY = 1.20f;
			weaponPanelTransform->SetScale(math::Vector3(weaponPanelScaleX, weaponPanelScaleY, 1.0f));
			float weaponWidthScaleHalf = weaponPanelScaleX / 2.0f;
			float weaponHeightScaleHalf = weaponPanelScaleY / 2.0f;
			float dashPanePadding = 0.150f;

			Vector3 weaponPanelPosition 
				= Vector3(
					MaxPositionX - weaponWidthScaleHalf - dashPanePadding
					, -MaxPositionY + weaponHeightScaleHalf + dashPanePadding, HUDZ);
			weaponPanelTransform->SetPosition(weaponPanelPosition);
		}
		// mouse
		{
			GameObject* cursorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"ShootingCursorMaterial");
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
			inventoryObject->GetTransform()->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			inventoryObject->GetTransform()->SetPosition(math::Vector3(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ));
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();
			inventoryObject->SetObjectState(GameObject::eObjectState::Hide);
		}

#pragma endregion

	}
}
