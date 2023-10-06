#include "daScene_Title.h"
#include "daApplication.h"

// Init
#include "daSceneManager.h"
#include "daGameDataManager.h"
#include "daItemManager.h"
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

#include "daAudioSource.h"
#include "daAudioScript.h"
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
		GameObject* light = SceneManager::GetLightObject();
		light->GetComponent<Light>()->SetColor(math::Vector4(0.70f, 0.70f, 0.70f, 1.0f));
		SceneManager::GetPlayerScript()->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
		SceneManager::GetHUDObject()->SetObjectStates(GameObject::eObjectState::Inactive);
		GameDataManager::SetCameraMovableRange(math::Vector2(0.0f, 0.0f));
		GameDataManager::SetCameraMovaPosition(math::Vector2::Zero);

		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Play(Resources::Find<AudioClip>(L"title"), 40.0f, true);
	}
	void Scene_Title::OnExit()
	{
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
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
		lightObj->AddComponent<AudioSource>();
		lightObj->AddComponent<AudioScript>();

		GameObject* audioListenerObj = objects::InstantiateCommonObject
			<GameObject>(this, enums::eLayerType::Default, L"NoneMaterial");
		SceneManager::AddSoundListenerObject(audioListenerObj);
		audioListenerObj->AddComponent<AudioScript>();
		audioListenerObj->AddComponent<AudioListener>();
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
		inventoryObject->AddComponent<AudioSource>();
		inventoryObject->SetObjectState(GameObject::eObjectState::Inactive);
		SceneManager::SetInventoryObject(inventoryObject);
		// Inventory Base
		{
			// 패널 Transform 세팅
			float inventoryScaleX = 1.230f;
			float inventoryScaleY = 1.80f;
			Transform* inventoryTransform = inventoryObject->GetTransform();
			inventoryTransform->SetScale(math::Vector3(inventoryScaleX * 4.0f, inventoryScaleY * 4.0f, 1.0f));
			math::Vector3 inventoryPosition(MaxPositionX - (inventoryScaleX * 2.0f), 0.0f, OverlayZ);
			inventoryTransform->SetPosition(inventoryPosition);
			
			// 인벤 스크립트 세팅
			InventoryScript* inventoryScript = inventoryObject->AddComponent<InventoryScript>();
			SceneManager::SetInventoryScript(inventoryObject);
			inventoryScript->SetSlotTextures(Resources::Find<graphics::Texture>(L"InventoryPanelATexture"), Resources::Find<graphics::Texture>(L"InventoryPanelBTexture"));

			float slotPosZ = -0.0040f;

			// SetA
			{
				// 슬롯 오브젝트 생성
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"WeaponSlot1Material");
				// 인벤토리에 슬롯 오브젝트를 자식으로 추가
				inventoryObject->AddChildObject(slotObject);
				// 슬롯오브젝트에 슬롯스크립트를 추가
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				// 스크립트에 텍스쳐 2개를 할당
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"WeaponSlotTexture"), Resources::Find<Texture>(L"WeaponSlotSelectTexture"));

				// Slot Transform 세팅 | 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.080f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				// 슬롯의 스크린 위치값을 저장
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ShiledSlot1Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ShiledSlotTexture"), Resources::Find<Texture>(L"ShiledSlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x + 0.040f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			// SetB
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"WeaponSlot2Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"WeaponSlotTexture"), Resources::Find<Texture>(L"WeaponSlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + slotSize.x, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, slotPosZ);
				slotTransform->SetPosition(slotPos);

				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ShiledSlot2Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ShiledSlotTexture"), Resources::Find<Texture>(L"ShiledSlotSelectTexture"));
				

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.0f) + 0.120f, inventoryPosition.y + (slotSize.y * 2.50f) + 0.080f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			// accessory
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"AccessorySlot1Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"AccessorySlotTexture"), Resources::Find<Texture>(L"AccessorySlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 1.50f) + 0.020f, inventoryPosition.y + slotSize.y + 0.10f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"AccessorySlot2Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"AccessorySlotTexture"), Resources::Find<Texture>(L"AccessorySlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 0.50f) + 0.060f, inventoryPosition.y + slotSize.y + 0.10f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"AccessorySlot3Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"AccessorySlotTexture"), Resources::Find<Texture>(L"AccessorySlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 0.50f) + 0.10f, inventoryPosition.y + slotSize.y + 0.10f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"AccessorySlot4Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"AccessorySlotTexture"), Resources::Find<Texture>(L"AccessorySlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) + 0.140f, inventoryPosition.y + slotSize.y + 0.10f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			// item
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot00Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot01Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot02Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot03Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot04Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 0.50f) + 0.040f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}

			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot10Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot11Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot12Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot13Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot14Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 1.50f) - 0.120f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}

			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot20Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - (slotSize.x * 2.0f) - 0.160f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot21Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x - slotSize.x - 0.040f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot22Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));


				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + 0.080f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot23Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 1.50f) - 0.180f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}
			{
				GameObject* slotObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ItemSlot24Material");
				inventoryObject->AddChildObject(slotObject);
				ItemSlotScript* itemSlotScript = inventoryScript->AddItemSlotScript(slotObject);
				itemSlotScript->SetSlotTextures(Resources::Find<Texture>(L"ItemSlotTexture"), Resources::Find<Texture>(L"ItemSlotSelectTexture"));

				// 19 * 4
				Transform* slotTransform = slotObject->GetComponent<Transform>();
				math::Vector3 slotSize(0.190f * 4.0f, 0.190f * 4.0f, 1.0f);
				slotTransform->SetScale(slotSize);
				math::Vector3 slotPos = math::Vector3(inventoryPosition.x + (slotSize.x * 2.50f) - 0.060f, inventoryPosition.y - (slotSize.y * 2.50f) - 0.240f, slotPosZ);
				slotTransform->SetPosition(slotPos);
				itemSlotScript->SetScreenPosision();
			}

			// player Amour panel A, B
			{
				// Panel A 생성
				GameObject* armourPanelAObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"Armour1Material");
				playerHUD->AddChildObject(armourPanelAObject);
				ArmourScript* armourScriptA = inventoryScript->AddArmourScript(armourPanelAObject);
				GameObject* iconObjectA = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ArmourIconAMaterial");
				armourScriptA->AddIconScript(iconObjectA);
				playerHUD->AddChildObject(iconObjectA);


				// Scale Position 세팅 34 24
				Transform* weaponPanelATransform = armourPanelAObject->GetTransform();
				float weaponPanelScaleX = 0.340f * 4.0f;
				float weaponPanelScaleY = 0.240f * 4.0f;
				float armourPadding = 0.20f;
				math::Vector3 armourPanelScale(weaponPanelScaleX, weaponPanelScaleY, 1.0f);
				math::Vector3 armourPanelPosition(MaxPositionX - (weaponPanelScaleX / 2.0f) - armourPadding * 2.0f
					, -MaxPositionY + (weaponPanelScaleY / 2.0f) + armourPadding, HUDZ);
				weaponPanelATransform->SetScale(armourPanelScale);
				weaponPanelATransform->SetPosition(armourPanelPosition);

				// Panel B 생성
				GameObject* armourPanelBObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"Armour2Material");
				playerHUD->AddChildObject(armourPanelBObject);
				ArmourScript* armourScriptB = inventoryScript->AddArmourScript(armourPanelBObject);
				armourScriptB->SetBackup(true);
				GameObject* iconObjectB = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"ArmourIconBMaterial");
				armourScriptB->AddIconScript(iconObjectB);
				playerHUD->AddChildObject(iconObjectB);


				Transform* weaponPanelBTransform = armourPanelBObject->GetTransform();
				weaponPanelBTransform->SetScale(armourPanelScale);
				weaponPanelBTransform->SetPosition(armourPanelPosition + math::Vector3(armourPadding, armourPadding, 0.0001f));
				// A, B Padding 차이 // X : armourPadding * 3, Y : armourPadding
			}

			GameObject* defaultItemObject = objects::InstantiateCommonObject<GameObject>(this, enums::eLayerType::UI, L"WeaponIconMaterial");
			defaultItemObject->SetObjectState(GameObject::eObjectState::Inactive);
			ItemScript* defalutWeapon = defaultItemObject->AddComponent<ItemScript>();

			// Item Setting
			defalutWeapon->SetItemName(L"LongSword");
			defalutWeapon->SetItemRenderType(enums::eItemRenderType::Texture);
			structs::sAttackStat defalutWeaponStat;
			defalutWeaponStat.AtaackDamage = 8.0f;
			defalutWeaponStat.AttackDelayTime.End = 0.350f;
			defalutWeapon->SetItemAttackStat(defalutWeaponStat);
			defalutWeapon->SetItemScale(math::Vector3(0.360f * 0.760f, 1.060f * 0.760f, 1.0f));			
			defalutWeapon->SetItemTexture(Resources::Find<Texture>(L"LongSword"));
			// Item Unit Setting
			structs::sUnitTypes defalutWeaponTypes;
			defalutWeaponTypes.ActionType = enums::eUnitActionType::None;
			defalutWeaponTypes.RenderType = enums::eUnitRenderType::JustRotate;
			defalutWeaponTypes.UsageType = enums::eUnitUsageType::Default;
			defalutWeapon->SetItemUnitTypes(defalutWeaponTypes);
			structs::sActionUnitInfo defalutWeaponInfo;
			defalutWeaponInfo.DurationTime = structs::sActionTimeValues(); // 기본값 0 - 1
			defalutWeaponInfo.Range = 0.50f;
			defalutWeaponInfo.Speed = 0.0f;
			defalutWeapon->SetItemUnitInfo(defalutWeaponInfo);
			defalutWeapon->SetItemUnitScale(math::Vector3(1.60f, 1.60f, 1.0f));
			defalutWeapon->SetItemUnitOffset(math::Vector3(0.50f, 0.50f, 0.0f));
			structs::sAnimationInfo unitAnimation;
			unitAnimation.Name = L"Swing";
			unitAnimation.Loop = false;
			defalutWeapon->SetItemUnitAnimation(unitAnimation);

			ItemManager::AddItem(L"LongSword", defalutWeapon);

			inventoryScript->AddItemToSlot(enums::eItemSlot::Slot00, ItemManager::GetItem(L"LongSword"));
			inventoryScript->SetPlayerScript(playerObject);
		}
#pragma endregion
	}
}
