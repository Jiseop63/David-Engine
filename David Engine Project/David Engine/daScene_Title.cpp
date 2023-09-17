#include "daScene_Title.h"
#include "daApplication.h"

// Init
#include "daSceneManager.h"
#include "daGameDataManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"
#include "daRenderer.h"

// objects & components
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"
#include "daTimeConstants.h"

#include "daPaintShader.h"
#include "daParticleRenderer.h"



extern da::Application application;

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
		initializeCommonObjects();
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
		// 각종 객체들 Inactive 해주기
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.70f, 0.70f, 0.70f, 1.0f));
		SceneManager::GetPlayerScript()->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
		SceneManager::GetHUDObject()->SetObjectStates(GameObject::eObjectState::Inactive);
		GameDataManager::SetCameraMovableRange(math::Vector2(0.0f, 0.0f));
		GameDataManager::SetCameraMovaPosition(math::Vector2::Zero);
	}
	void Scene_Title::OnExit()
	{
	}

	void Scene_Title::addBackgroundObjects()
	{
		float width = MaxPositionX * 2.0f;
		float height = MaxPositionY * 2.0f;
		// just BG
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"TitleBackgroundMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(width, height, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
		}
		// moving background layer
		{
			// Back & Front Cloud Layer
			GameObject* cloudObj = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"BackCloudMaterial");
			cloudObj->GetTransform()->SetScale(math::Vector3(width, height, 1.0f));
			cloudObj->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackLayerZ));
			TimeConstants* cloudTimer = cloudObj->AddComponent<TimeConstants>();
			cloudTimer->SetValue(0.05f);


			GameObject* cloudObj2 = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"FrontCloudMaterial");
			cloudObj2->GetTransform()->SetScale(math::Vector3(width, height, 1.0f));
			cloudObj2->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
			TimeConstants* cloudTimer2 = cloudObj2->AddComponent<TimeConstants>();
			cloudTimer2->SetValue(0.3f);

		}
		// BGObj MainLogo
		{
			GameObject* titleLogoObj = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"MainLogoMaterial");
			titleLogoObj->GetTransform()->SetScale(math::Vector3(4.680f, 2.250f, 1.0f));
			titleLogoObj->GetTransform()->SetPosition(math::Vector3(0.0f, (height / 2.0f) - 2.250f, HUDZ));
		}
		// BGObj Copyright
		{
			GameObject* copyrightObj = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"CopyrightMaterial");
			copyrightObj->GetTransform()->SetScale(math::Vector3(5.010f, 0.360f, 1.0f));
			copyrightObj->GetTransform()->SetPosition(math::Vector3(0.0f, -(height / 2.0f) + 0.360f, HUDZ));
		}		
	}
	void Scene_Title::addUIObjects()
	{
		float Width = (float)application.GetClientWidth();
		Width /= 100.0f;
		float height = (float)application.GetClientHeight();
		height /= 100.0f;

		// buttons
		{
			float playBtnScaleX = 1.440f;
			float playBtnScaleY = 0.480f;
			UIObject* playBtnObject = objects::InstantiateButtonObject<UIObject>(this, L"StartBtnMaterial", L"PlayOffTexture", L"PlayOnTexture");
			playBtnObject->GetTransform()->SetScale(math::Vector3(playBtnScaleX, playBtnScaleY, 1.0f));
			playBtnObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
			ButtonScript* playBtnScript = playBtnObject->GetComponent<ButtonScript>();
			playBtnScript->SetScreenPosision();
			playBtnScript->SetButtonType(ButtonScript::eButtonType::Play);
			UIObject* exitBtnObject = objects::InstantiateButtonObject<UIObject>(this, L"ExitBtnMaterial", L"ExitOffTexture", L"ExitOnTexture");
			exitBtnObject->GetTransform()->SetScale(math::Vector3(0.840f, 0.480f, 1.0f));
			exitBtnObject->GetTransform()->SetPosition(math::Vector3(0.0f, -1.0f, BackgroundZ));
			ButtonScript* exitBtnScript = exitBtnObject->GetComponent<ButtonScript>();
			exitBtnScript->SetScreenPosision();
			exitBtnScript->SetButtonType(ButtonScript::eButtonType::Exit);
		}

		
	}
	void Scene_Title::initializeCommonObjects()
	{
		// camera Init
		CameraObject* subCameraObj = objects::InstantiateSubCamera(this);
		SceneManager::AddSubCameraObject(subCameraObj);
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		SceneManager::AddMainCameraObject(mainCameraObj);
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		renderer::mainCamera = mainCameraObj->GetCameraComponent();
		renderer::uiCamera = uiCameraObj->GetCameraComponent();
		// Camera setting
		SubCameraScript* subCamScript = subCameraObj->GetComponent<SubCameraScript>();
		subCamScript->SetMainCameraTransfrom(mainCameraObj->GetTransform());
		/*AudioSource* audioSource = mainCameraObj->AddComponent<AudioSource>();
		audioSource->SetClip(Resources::Load<AudioClip>(L"TestSound", L"..\\Resources\\Sound\\0.mp3"));
		audioSource->Play();*/

		// light - done
		GameObject* lightObj = objects::InstantiateCommonObject
			<GameObject>(this, enums::eLayerType::Light, L"NoneMaterial");
		SceneManager::AddLightObject(lightObj);
		Light* light = lightObj->AddComponent<Light>();
		light->SetLightType(enums::eLightType::Directional);
		light->SetColor(math::Vector4(0.90f, 0.90f, 0.90f, 1.0f));

		// player - done
		GameObject* playerObject = objects::InstantiatePlayer(this);
		SceneManager::AddPlayerObject(playerObject);
#pragma region HUD

		// hud - done
		GameObject* playerHUD = objects::InstantiateObject
			<GameObject>(this, enums::eLayerType::Default);
		playerHUD->SetCommonObject(true);
		SceneManager::AddHUDObject(playerHUD);
		{
			// hpBar, dashBar
			{
				Transform* playerHUDTransform = playerHUD->GetTransform();
				//  HUD 위치 조절 (좌상단)
				playerHUDTransform->SetPosition(math::Vector3(-MaxPositionX, MaxPositionY, HUDZ));
				float padding = 0.20f;


				// Life Panel 객체 생성
				GameObject* lifePanel = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
				playerHUD->AddChildObject(lifePanel);
				Transform* lifePanelTransform = lifePanel->GetTransform();
				lifePanelTransform->SetParent(playerHUDTransform);
				// Life Panel 크기 조절
				float panelXScale = 2.960f;	// 74 * 4
				float panelYScale = 0.640f;	// 16 * 4
				lifePanelTransform->SetScale(math::Vector3(panelXScale, panelYScale, 1.0f));
				// Life Panel 위치 조절
				math::Vector3 lifePanelPosition = math::Vector3((panelXScale / 2.0f) + padding, - (panelYScale / 2.0f) - padding, 0.f);
				lifePanelTransform->SetPosition(lifePanelPosition);

				// Bar 객체 생성
				GameObject* lifeBar = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
				playerHUD->AddChildObject(lifeBar);

				// 플레이어 hp 연동
				PlayerScript* playerScript = SceneManager::GetPlayerScript();
				LifeBarScript* playerLifeBar = lifeBar->AddComponent<LifeBarScript>();
				playerLifeBar->SetValue(&playerScript->GetCreatureStat()->MaxHP, &playerScript->GetCreatureStat()->CurHP);
				SceneManager::AddLifebarObject(lifeBar);

				// Bar 크기 조절
				Transform* lifeBarTransform = lifeBar->GetTransform();
				lifeBarTransform->SetParent(playerHUDTransform);
				float barXScale = 1.960f;	// 49 * 4
				float barYScale = 0.40f;	// 10 * 4
				lifeBarTransform->SetScale(math::Vector3(barXScale, barYScale, 1.0f));
				// Bar 위치 조절하기
				// 22 * 4
				// hud 위치에서 panel size의 절반만큼 당긴다음에... bar offset만큼 더 당기면될듯?
				math::Vector3 lifeBarPosition = lifePanelPosition + math::Vector3(0.380f, 0.0f, -0.0001f);
				lifeBarTransform->SetPosition(lifeBarPosition);

				// Dash Panel 객체 생성
				GameObject* dashPanel = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");
				playerHUD->AddChildObject(dashPanel);
				Transform* dashPanelTransform = dashPanel->GetTransform();
				dashPanelTransform->SetParent(playerHUDTransform);

				// Dash Panel 크기 조절
				// scale
				float dashPanelXScale = 0.880f;	// 22 * 4
				float dashPanelYScale = 0.320f;	// 8 * 4
				dashPanelTransform->SetScale(math::Vector3(dashPanelXScale, dashPanelYScale, 1.0f));

				// Dash Panel 위치 조절
				math::Vector3 dashPanelPosition = math::Vector3((dashPanelXScale / 2.0f) + padding, lifePanelPosition.y - (dashPanelYScale / 2.0f) - (padding * 2.0f), 0.f);
				dashPanelTransform->SetPosition(dashPanelPosition);

				// Dash Active 객체 생성
				GameObject* dashActivate = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"DashActivateMaterial");
				playerHUD->AddChildObject(dashActivate);
				DashCountScript* dashCountScript = dashActivate->AddComponent<DashCountScript>();
				dashCountScript->SetName(L"debugingTarget");

				const structs::sDashData* datas = playerScript->GetDashData();
				dashCountScript->SetValue(&playerScript->GetDashData()->MaxDashCount, &playerScript->GetDashData()->CurDashCount);

				SceneManager::AddDashCountObject(dashActivate);
				Transform* dashActivateTransform = dashActivate->GetTransform();
				dashActivateTransform->SetParent(playerHUDTransform);

				// Dash Active scale
				float dashActiveXScale = 0.720f; // 18 * 4
				float dashActiveYScale = 0.160f; // 4 * 4
				dashActivateTransform->SetScale(math::Vector3(dashActiveXScale, dashActiveYScale, 1.0f));

				// Dash Active position
				math::Vector3 dashActivePosition = dashPanelPosition + math::Vector3(0.0f, 0.0f, -0.0001f);
				dashActivateTransform->SetPosition(dashActivePosition);
			}
			// player Amour panel A, B
			{
				// Panel A 생성
				GameObject* weaponPanelA = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour1Material");
				playerHUD->AddChildObject(weaponPanelA);
				weaponPanelA->AddComponent<ArmourScript>();
				// Scale Position 세팅 34 24
				Transform* weaponPanelATransform = weaponPanelA->GetTransform();
				float weaponPanelScaleX = 0.340f * 4.0f;
				float weaponPanelScaleY = 0.240f * 4.0f;
				float armourPadding = 0.20f;

				math::Vector3 armourPanelScale(weaponPanelScaleX, weaponPanelScaleY, 1.0f);
				math::Vector3 armourPanelPosition(MaxPositionX - (weaponPanelScaleX / 2.0f) - armourPadding * 2.0f
					, -MaxPositionY + (weaponPanelScaleY / 2.0f) + armourPadding, HUDZ);

				weaponPanelATransform->SetScale(armourPanelScale);
				weaponPanelATransform->SetPosition(armourPanelPosition);

				// Panel B 생성
				GameObject* weaponPanelB = objects::InstantiateCommonObject
					<GameObject>(this, enums::eLayerType::UI, L"Armour2Material");
				playerHUD->AddChildObject(weaponPanelB);
				ArmourScript* armourBScript = weaponPanelB->AddComponent<ArmourScript>();
				armourBScript->SetBackup(true);
				Transform* weaponPanelBTransform = weaponPanelB->GetTransform();

				weaponPanelBTransform->SetScale(armourPanelScale);
				weaponPanelBTransform->SetPosition(
					armourPanelPosition + math::Vector3(armourPadding, armourPadding, 0.0001f));
				// A, B Padding 차이
				// X : armourPadding * 3, Y : armourPadding
			}
		}
		// mouse - done
		GameObject* cursorObject = objects::InstantiateCommonObject
			<GameObject>(this, enums::eLayerType::UI, L"CursorMaterial");
		cursorObject->AddComponent<CursorScript>();
		SceneManager::AddCursourObject(cursorObject);
		{
			cursorObject->GetTransform()->SetScale(math::Vector3(0.190f * 4.0f, 0.190f * 4.0f, 1.0f));
			cursorObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CursorZ));
		}


#pragma endregion
#pragma region Inventory
		GameObject* inventoryObject = objects::InstantiateCommonObject<GameObject>
			(this, enums::eLayerType::UI, L"InventoryPanelMaterial");
		SceneManager::SetInventoryObject(inventoryObject);
		// Inventory Base
		{
			float inventoryScaleX = 1.230f;
			float inventoryScaleY = 1.80f;
			Transform* inventoryTransform = inventoryObject->GetTransform();
			inventoryTransform->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			math::Vector3 inventoryPosition(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ);
			inventoryTransform->SetPosition(inventoryPosition);
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();

			SceneManager::SetInventoryScript(inventoryObject);

			inventoryScript->SetSlotTextures(Resources::Find<graphics::Texture>(L"InventoryPanelATexture"), Resources::Find<graphics::Texture>(L"InventoryPanelBTexture"));
			inventoryObject->SetObjectState(GameObject::eObjectState::Hide);

			// SetA
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"WeaponSlot1Material", L"WeaponSlotTexture", L"WeaponSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.080f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddWeaonSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ShiledSlot1Material", L"ShiledSlotTexture", L"ShiledSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x + 0.040f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddShiledSlot(slotObject);
			}
			// SetB
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"WeaponSlot2Material", L"WeaponSlotTexture", L"WeaponSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + slotSize.x, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddWeaonSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ShiledSlot2Material", L"ShiledSlotTexture", L"ShiledSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.0f) + 0.120f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddShiledSlot(slotObject);
			}
			// accessory
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot1Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 1.50f) + 0.020f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot2Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 0.50f) + 0.060f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot3Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 0.50f) + 0.10f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"AccessorySlot4Material", L"AccessorySlotTexture", L"AccessorySlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) + 0.140f, inventoryPosition.y + slotSize.y + 0.10f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddAccessorySlot(slotObject);
			}
			// item
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot00Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot01Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot02Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot03Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot04Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}

			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot10Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot11Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot12Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot13Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot14Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}

			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot20Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot21Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot22Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot23Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
			{
				GameObject* slotObject = objects::InstantiateMultiTextureUI<GameObject>
					(this, L"ItemSlot24Material", L"ItemSlotTexture", L"ItemSlotSelectTexture");
				inventoryObject->AddChildObject(slotObject);
				slotObject->SetObjectState(GameObject::eObjectState::Inactive);
				slotObject->SetCommonObject(true);
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				// 19 * 4
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, inventoryPosition.z);
				slotTransform->SetPosition(slotPos);
				slotObject->GetComponent<UIScript>()->SetScreenPosision();
				inventoryScript->AddItemSlot(slotObject);
			}
		}
#pragma endregion

		
	}
}
