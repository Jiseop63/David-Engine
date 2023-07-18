#include "daScene_Dungeon1F.h"

#include "daRenderer.h"

// 임시
#include "daInput.h"

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
		CameraObject* subCameraObj = objects::InstantiateSubCamera(this);
		mMainCamera = objects::InstantiateMainCamera(this);
		// subCamera setting
		SubCameraScript* subCamScript = subCameraObj->GetComponent<SubCameraScript>();
		subCamScript->SetMainCameraTransfrom(mMainCamera->GetTransform());
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

		// HUD 객체 생성
		{
			// hpBar
			{
				GameObject* playerHUD = objects::InstantiateObject
					<GameObject>(this, enums::eLayerType::None);
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
				Vector3 dashPanelPosition = Vector3((dashPanelXScale / 2.0f) + padding, lifePanelPosition.y - (dashPanelYScale / 2.0f) - (padding * 2.0f), 0.f);
				dashPanelTransform->SetPosition(dashPanelPosition);

				// Dash Active 객체 생성
				GameObject* dashActivate = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"DashActivateMaterial");
				Transform* dashActivateTransform = dashActivate->GetTransform();
				dashActivateTransform->SetParent(playerHUDTransform);

				// Dash Active scale
				float dashActiveXScale = 0.720f; // 18 * 4
				float dashActiveYScale = 0.160f; // 4 * 4
				dashActivateTransform->SetScale(Vector3(dashActiveXScale, dashActiveYScale, 1.0f));

				// Dash Active position
				Vector3 dashActivePosition = dashPanelPosition + Vector3(0.0f, 0.0f, -0.0001f);
				dashActivateTransform->SetPosition(dashActivePosition);
				dashActivate->AddComponent<DashCountScript>();
			}
		}

		// player weapon panel
		{
			GameObject* weaponPanel = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			weaponPanel->SetName(L"weaponPanel");
			Transform* weaponPanelTransform = weaponPanel->GetTransform();
			float weaponPanelScaleX = 1.70f;
			float weaponPanelScaleY = 1.0f;
			weaponPanelTransform->SetScale(math::Vector3(weaponPanelScaleX, weaponPanelScaleY, 1.0f));
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
				(this, enums::eLayerType::UI, L"InventoryPanelMaterial");
			inventoryObject->SetName(L"inventory");
			Transform* inventoryTransform = inventoryObject->GetTransform();
			inventoryTransform->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			Vector3 inventoryPosition(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ);
			inventoryTransform->SetPosition(inventoryPosition);
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();
			inventoryScript->SetUITextures(Resources::Find<graphics::Texture>(L"InventoryPanelATexture"), Resources::Find<graphics::Texture>(L"InventoryPanelBTexture"));
			inventoryObject->SetObjectState(GameObject::eObjectState::Hide);

			// SetA
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"WeaponSlot1Material", L"WeaponSlotTexture", L"WeaponSlotSelectTexture");
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
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
}
