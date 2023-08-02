#include "daScene_Dungeon2F_Stage2.h"
// �ӽ�
#include "daInput.h"
#include "daGameDataManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"
#include "daRenderer.h"

// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"

namespace da
{
	Scene_Dungeon2F_Stage2::Scene_Dungeon2F_Stage2()
	{
	}
	Scene_Dungeon2F_Stage2::~Scene_Dungeon2F_Stage2()
	{
	}
	void Scene_Dungeon2F_Stage2::Initialize()
	{
		CameraObject* subCameraObj = objects::InstantiateSubCamera(this);
		mMainCamera = objects::InstantiateMainCamera(this);
		// subCamera setting
		SubCameraScript* subCamScript = subCameraObj->GetComponent<SubCameraScript>();
		subCamScript->SetMainCameraTransfrom(mMainCamera->GetTransform());
		mUICamera = objects::InstantiateUICamera(this);

		addBackgroundObjects();
		//addUIObjects();
		addGameObjects();
	}
	void Scene_Dungeon2F_Stage2::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Title");
		}
		Scene::Update();
	}
	void Scene_Dungeon2F_Stage2::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon2F_Stage2::Render()
	{
		Scene::Render();
	}
	void Scene_Dungeon2F_Stage2::OnEnter()
	{
	}
	void Scene_Dungeon2F_Stage2::OnExit()
	{
	}
	void Scene_Dungeon2F_Stage2::addBackgroundObjects()
	{
	}
	void Scene_Dungeon2F_Stage2::addUIObjects()
	{
#pragma region HUD

		// HUD ��ü ����
		{
			// hpBar, dashBar
			{
				GameObject* playerHUD = objects::InstantiateObject
					<GameObject>(this, enums::eLayerType::Default);
				playerHUD->SetName(L"lifeHUD");
				Transform* playerHUDTransform = playerHUD->GetTransform();
				//  HUD ��ġ ���� (�»��)
				playerHUDTransform->SetPosition(Vector3(-MaxPositionX, MaxPositionY, HUDZ));
				float padding = 0.20f;


				// Life Panel ��ü ����
				GameObject* lifePanel = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
				lifePanel->SetName(L"lifePanel");
				Transform* lifePanelTransform = lifePanel->GetTransform();
				lifePanelTransform->SetParent(playerHUDTransform);
				// Life Panel ũ�� ����
				float panelXScale = 2.960f;	// 74 * 4
				float panelYScale = 0.640f;	// 16 * 4
				lifePanelTransform->SetScale(Vector3(panelXScale, panelYScale, 1.0f));
				// Life Panel ��ġ ����
				Vector3 lifePanelPosition = Vector3((panelXScale / 2.0f) + padding, -(panelYScale / 2.0f) - padding, 0.f);
				lifePanelTransform->SetPosition(lifePanelPosition);

				// Bar ��ü ����
				GameObject* lifeBar = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
				lifeBar->SetName(L"lifeBar");
				mLifeBar = lifeBar;
				Transform* lifeBarTransform = lifeBar->GetTransform();
				lifeBarTransform->SetParent(playerHUDTransform);
				// Bar ũ�� ����
				float barXScale = 1.960f;	// 49 * 4
				float barYScale = 0.40f;	// 10 * 4
				lifeBarTransform->SetScale(Vector3(barXScale, barYScale, 1.0f));
				// Bar ��ġ �����ϱ�
				// 22 * 4
				// hud ��ġ���� panel size�� ���ݸ�ŭ ��������... bar offset��ŭ �� ����ɵ�?
				Vector3 lifeBarPosition = lifePanelPosition + Vector3(0.380f, 0.0f, -0.0001f);
				lifeBarTransform->SetPosition(lifeBarPosition);
				lifeBar->AddComponent<LifeBarScript>();


				// Dash Panel ��ü ����
				GameObject* dashPanel = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");

				Transform* dashPanelTransform = dashPanel->GetTransform();
				dashPanelTransform->SetParent(playerHUDTransform);

				// Dash Panel ũ�� ����
				// scale
				float dashPanelXScale = 0.880f;	// 22 * 4
				float dashPanelYScale = 0.320f;	// 8 * 4
				dashPanelTransform->SetScale(Vector3(dashPanelXScale, dashPanelYScale, 1.0f));

				// Dash Panel ��ġ ����
				Vector3 dashPanelPosition = Vector3((dashPanelXScale / 2.0f) + padding, lifePanelPosition.y - (dashPanelYScale / 2.0f) - (padding * 2.0f), 0.f);
				dashPanelTransform->SetPosition(dashPanelPosition);

				// Dash Active ��ü ����
				GameObject* dashActivate = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"DashActivateMaterial");
				mDashCountBar = dashActivate;

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

			// player Amour panel A, B
			{
				// Panel A ����
				GameObject* weaponPanelA = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour1Material");
				weaponPanelA->SetName(L"weaponPanelA");
				weaponPanelA->AddComponent<ArmourScript>();
				// Scale Position ���� 34 24
				Transform* weaponPanelATransform = weaponPanelA->GetTransform();
				float weaponPanelScaleX = 0.340f * 4.0f;
				float weaponPanelScaleY = 0.240f * 4.0f;
				float armourPadding = 0.20f;

				Vector3 armourPanelScale(weaponPanelScaleX, weaponPanelScaleY, 1.0f);
				Vector3 armourPanelPosition(MaxPositionX - (weaponPanelScaleX / 2.0f) - armourPadding * 2.0f
					, -MaxPositionY + (weaponPanelScaleY / 2.0f) + armourPadding, HUDZ);

				weaponPanelATransform->SetScale(armourPanelScale);
				weaponPanelATransform->SetPosition(armourPanelPosition);

				// Panel B ����
				GameObject* weaponPanelB = objects::InstantiateGameObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour2Material");
				weaponPanelB->SetName(L"weaponPanelB");
				ArmourScript* armourBScript = weaponPanelB->AddComponent<ArmourScript>();
				armourBScript->SetBackup(true);
				Transform* weaponPanelBTransform = weaponPanelB->GetTransform();

				weaponPanelBTransform->SetScale(armourPanelScale);
				weaponPanelBTransform->SetPosition(
					armourPanelPosition + Vector3(armourPadding, armourPadding, 0.0001f));

				// A, B Padding ����
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
	void Scene_Dungeon2F_Stage2::addGameObjects()
	{
		// Stage : 352 304
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"2FStage2Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.520f * 4.0f, 3.040f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
		}
		//// player
		//{
		//	GameObject* playerObject = objects::InstantiatePlayer(this, L"AnimationMaterial");
		//	playerObject->SetName(L"player");
		//	PlayerScript* playerScript = playerObject->GetComponent<PlayerScript>();
		//	mPlayer = playerObject;

		//	Light* light = playerObject->AddComponent<Light>();
		//	light->SetRadius(4.0f);
		//	light->SetLightType(enums::eLightType::Point);
		//	light->SetColor(math::Vector4(0.60f, 0.60f, 0.60f, 1.0f));


		//	GameObject* weaponObject
		//		= objects::InstantiateGameObject<GameObject>
		//		(this, enums::eLayerType::PlayableAttackCollider, L"WeaponMaterial");
		//	mWeapon = weaponObject;
		//	WeaponScript* weaponScript = playerScript->SetWeaponObject(weaponObject);


		//	GameObject* effectObject
		//		= objects::InstantiateGameObject<GameObject>
		//		(this, enums::eLayerType::Effect, L"AnimationMaterial");
		//	weaponScript->AddEffectObject(effectObject);
		//}

		//// test enemy
		//{
		//	GameObject* monsterObject = objects::InstantiateCreature<GameObject>(this, L"AnimationMaterial");
		//	monsterObject->SetName(L"monster");
		//	monsterObject->GetTransform()->SetPosition(Vector3(1.50f, 0.0f, ObjectZ));
		//	monsterObject->AddComponent<CreatureScript>();
		//}

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
		light->SetColor(math::Vector4(0.50f, 0.50f, 0.50f, 1.0f));
	}
}