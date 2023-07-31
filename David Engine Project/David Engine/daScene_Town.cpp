#include "daScene_Town.h"

#include "daRenderer.h"
// 임시
#include "daInput.h"
#include "daApplication.h"
#include "daResources.h"
#include "daTexture.h"

#include "daObjects.h"
#include "daGameDataManager.h"

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
		// sub
		CameraObject* subCameraObj = objects::InstantiateSubCamera(this);
		// main
		mMainCamera = objects::InstantiateMainCamera(this);

		// subCamera setting
		SubCameraScript* subCamScript = subCameraObj->GetComponent<SubCameraScript>();
		subCamScript->SetMainCameraTransfrom(mMainCamera->GetTransform());

		// UI
		mUICamera = objects::InstantiateUICamera(this);

		addBackgroundObject();
		addUIObjects();
		addGameObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F_Stage1");
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
		renderer::gridScript->SetCamera(renderer::mainCamera);

		// 플레이어 세팅
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Creature);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Land);
		GameDataManager::SetInventoryObject(mInventory);
		GameDataManager::SetPlayerObject(mPlayer);
		GameDataManager::SetWeaponObject(mWeapon);
		GameDataManager::SetHPBar(mLifeBar);
		GameDataManager::SetDashCountBar(mDashCountBar);
	}
	void Scene_Town::OnExit()
	{
		CollisionManager::Clear();
	}

	void Scene_Town::addBackgroundObject()
	{
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
			backGround->SetName(L"sky");
		}

		// forest Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -2.0f, BackLayerZ));
			backGround->SetName(L"forest");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			bgLayerScript->SetCamera(mMainCamera->GetCameraComponent());
			bgLayerScript->SetName(L"첫번째 레이어 스크립트");
		}

		// trees Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -4.0f, FrontLayerZ));
			backGround->SetName(L"trees");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			bgLayerScript->SetCamera(mMainCamera->GetCameraComponent());
			bgLayerScript->SetName(L"두번째 레이어 스크립트");
		}
	}
	void Scene_Town::addUIObjects()
	{
#pragma region HUD
		
		// HUD 객체 생성
		{
			// hpBar, dashBar
			{
				GameObject* playerHUD = objects::InstantiateObject
					<GameObject>(this, enums::eLayerType::Default);
				playerHUD->SetName(L"lifeHUD");
				Transform* playerHUDTransform = playerHUD->GetTransform();
				//  HUD 위치 조절 (좌상단)
				playerHUDTransform->SetPosition(Vector3(-MaxPositionX, MaxPositionY, HUDZ));
				float padding = 0.20f;


				// Life Panel 객체 생성
				GameObject* lifePanel = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
				lifePanel->SetName(L"lifePanel");
				Transform* lifePanelTransform = lifePanel->GetTransform();
				lifePanelTransform->SetParent(playerHUDTransform);
				// Life Panel 크기 조절
				float panelXScale = 2.960f;	// 74 * 4
				float panelYScale = 0.640f;	// 16 * 4
				lifePanelTransform->SetScale(Vector3(panelXScale, panelYScale, 1.0f));
				// Life Panel 위치 조절
				Vector3 lifePanelPosition = Vector3((panelXScale / 2.0f) + padding, -(panelYScale / 2.0f) - padding, 0.f);
				lifePanelTransform->SetPosition(lifePanelPosition);

				// Bar 객체 생성
				GameObject* lifeBar = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
				lifeBar->SetName(L"lifeBar");
				mLifeBar = lifeBar;
				Transform* lifeBarTransform = lifeBar->GetTransform();
				lifeBarTransform->SetParent(playerHUDTransform);
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


				// Dash Panel 객체 생성
				GameObject* dashPanel = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");

				Transform* dashPanelTransform = dashPanel->GetTransform();
				dashPanelTransform->SetParent(playerHUDTransform);

				// Dash Panel 크기 조절
				// scale
				float dashPanelXScale = 0.880f;	// 22 * 4
				float dashPanelYScale = 0.320f;	// 8 * 4
				dashPanelTransform->SetScale(Vector3(dashPanelXScale, dashPanelYScale, 1.0f));

				// Dash Panel 위치 조절
				Vector3 dashPanelPosition = Vector3((dashPanelXScale / 2.0f) + padding, lifePanelPosition.y -(dashPanelYScale / 2.0f) -(padding * 2.0f), 0.f);
				dashPanelTransform->SetPosition(dashPanelPosition);
				
				// Dash Active 객체 생성
				GameObject* dashActivate = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"DashActivateMaterial");
				mDashCountBar = dashActivate;
				
				Transform* dashActivateTransform = dashActivate->GetTransform(); 
				dashActivateTransform->SetParent(playerHUDTransform);

				// Dash Active scale
				float dashActiveXScale = 0.720f; // 18 * 4
				float dashActiveYScale = 0.160f; // 4 * 4
				dashActivateTransform->SetScale( Vector3(dashActiveXScale, dashActiveYScale, 1.0f) );

				// Dash Active position
				Vector3 dashActivePosition = dashPanelPosition + Vector3(0.0f, 0.0f, -0.0001f);
				dashActivateTransform->SetPosition(dashActivePosition);
				dashActivate->AddComponent<DashCountScript>();
			}

			// player Amour panel A, B
			{
				// Panel A 생성
				GameObject* weaponPanelA = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour1Material");
				weaponPanelA->SetName(L"weaponPanelA");
				weaponPanelA->AddComponent<ArmourScript>();
				// Scale Position 세팅 34 24
				Transform* weaponPanelATransform = weaponPanelA->GetTransform();
				float weaponPanelScaleX = 0.340f * 4.0f;
				float weaponPanelScaleY = 0.240f * 4.0f;
				float armourPadding = 0.20f;

				Vector3 armourPanelScale(weaponPanelScaleX, weaponPanelScaleY, 1.0f);
				Vector3 armourPanelPosition(MaxPositionX - (weaponPanelScaleX / 2.0f) - armourPadding * 2.0f
						, -MaxPositionY + (weaponPanelScaleY / 2.0f) + armourPadding, HUDZ);

				weaponPanelATransform->SetScale(armourPanelScale);
				weaponPanelATransform->SetPosition(armourPanelPosition);
				
				// Panel B 생성
				GameObject* weaponPanelB = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour2Material");
				weaponPanelB->SetName(L"weaponPanelB");
				ArmourScript* armourBScript = weaponPanelB->AddComponent<ArmourScript>();
				armourBScript->SetBackup(true);
				Transform* weaponPanelBTransform = weaponPanelB->GetTransform();

				weaponPanelBTransform->SetScale(armourPanelScale);
				weaponPanelBTransform->SetPosition(
					armourPanelPosition + Vector3(armourPadding, armourPadding, 0.0001f));

				// A, B Padding 차이
				// X : armourPadding * 3, Y : armourPadding
			}
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
				(this, enums::eLayerType::UI, L"InventoryPanelMaterial");
			inventoryObject->SetName(L"inventory");
			mInventory = inventoryObject;
			Transform* inventoryTransform = inventoryObject->GetTransform();
			inventoryTransform->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			Vector3 inventoryPosition(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ);
			inventoryTransform->SetPosition(inventoryPosition);
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();
			inventoryScript->SetSlotTextures(Resources::Find<graphics::Texture>(L"InventoryPanelATexture"), Resources::Find<graphics::Texture>(L"InventoryPanelBTexture"));
			inventoryObject->SetObjectState(GameObject::eObjectState::Hide);

			// SetA
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"WeaponSlot1Material", L"WeaponSlotTexture", L"WeaponSlotSelectTexture");
				slotObject ->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.080f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddWeaonSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ShiledSlot1Material", L"ShiledSlotTexture", L"ShiledSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - slotSize.x + 0.040f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddShiledSlot(slotObject);
			}
			// SetB
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"WeaponSlot2Material", L"WeaponSlotTexture", L"WeaponSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + slotSize.x, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddWeaonSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ShiledSlot2Material", L"ShiledSlotTexture", L"ShiledSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 2.0f) + 0.120f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddShiledSlot(slotObject);
			}
			// accessory
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot1Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 1.50f) + 0.020f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot2Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 0.50f) + 0.060f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot3Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 0.50f) + 0.10f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot4Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 1.50f) + 0.140f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			// item
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot00Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot01Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot02Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot03Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot04Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}

			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot10Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot11Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot12Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot13Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot14Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}

			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot20Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot21Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot22Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot23Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot24Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				Vector3 slotPos = Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
		}

#pragma endregion
	}
	void Scene_Town::addGameObjects()
	{
		// floor
		{
			// 694 128
			GameObject* floorObject = objects::InstantiateGameObject<GameObject>(this, enums::eLayerType::FrontBackGround, L"TownMapTileMaterial");
			floorObject->SetName(L"Floor");
			Transform* floorTr = floorObject->GetTransform();
			floorTr->SetScale(Vector3(19.180f * 4.0f, 1.920f * 4.0f, 1.0f));
			floorTr->SetPosition(Vector3(0.0f, -4.0f, ObjectZ));
		}
		
		// player
		{
			GameObject* playerObject = objects::InstantiatePlayer(this, L"AnimationMaterial");
			playerObject->SetName(L"player");
			PlayerScript* playerScript = playerObject->GetComponent<PlayerScript>();
			mPlayer = playerObject;
			
			GameObject* weaponObject 
				= objects::InstantiateGameObject<GameObject>
				(this, enums::eLayerType::PlayableAttackCollider, L"WeaponMaterial");
			mWeapon = weaponObject;
			WeaponScript* weaponScript = playerScript->SetWeaponObject(weaponObject);


			GameObject* effectObject
				= objects::InstantiateGameObject<GameObject>
				(this, enums::eLayerType::Effect, L"AnimationMaterial");
			weaponScript->SetEffectObject(effectObject);
		}

		// test enemy
		{
			GameObject* monsterObject = objects::InstantiateCreature<GameObject>(this, L"AnimationMaterial");
			monsterObject->SetName(L"monster");
			monsterObject->GetTransform()->SetPosition( Vector3(1.50f, 0.0f, ObjectZ) );
			monsterObject->AddComponent<CreatureScript>();
		}

		// land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-1.0f, -2.50f, 0.0f), Vector3(10.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		GameObject* lightObj = objects::InstantiateGameObject
			<GameObject>(this, enums::eLayerType::Light, L"NoneMaterial");
		Light* light = lightObj->AddComponent<Light>();
		light->SetLightType(enums::eLightType::Directional);
		light->SetColor(math::Vector4(0.90f, 0.90f, 0.90f, 1.0f));
	}
}
