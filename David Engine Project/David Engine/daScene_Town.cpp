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
		addBackgroundObject();
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

		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->SetPlayerPosition(math::Vector3(0.0f, 2.50f, -0.1f));
		player->SetPlayerVelocity(math::Vector2::Zero);
		player->ChangeState(ePlayerState::Idle);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(false);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(22.0f, 1.80f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// UI 세팅
		structs::sPlayerStat playerStat = GameDataManager::GetPlayerStat();
		SceneManager::GetLifebarScript()->SetValue(playerStat.MaxHP, playerStat.CurHP);
		SceneManager::GetHUDObject()->SetObjectStates(GameObject::eObjectState::Active);
		SceneManager::GetCursourScrip()->IsIngame(true);
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.70f, 0.70f, 0.70f, 1.0f));
		// 충돌 세팅
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Playable);
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Creature);
		CollisionManager::SetLayer(enums::eLayerType::Platform, enums::eLayerType::Playable);
		CollisionManager::SetLayer(enums::eLayerType::Platform, enums::eLayerType::Creature);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Creature);
	}
	void Scene_Town::OnExit()
	{
		SceneManager::GetPlayerScript()->PlayerIsNotGround();
	}

	void Scene_Town::addBackgroundObject()
	{
		CameraObject* mainCameraObject = dynamic_cast<CameraObject*>(SceneManager::GetMainCameraScript()->GetOwner());
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 1.920f, BackgroundZ));
			backGround->SetName(L"sky");
		}
		// forest Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 1.0f, BackLayerZ));
			backGround->SetName(L"forest");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"첫번째 레이어 스크립트");
		}
		// trees Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
			backGround->SetName(L"trees");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"두번째 레이어 스크립트");
		}
		// floor
		{
			// 694 128
			GameObject* floorObject = objects::InstantiateGameObject<GameObject>(this, enums::eLayerType::FrontBackGround, L"TownMapTileMaterial");
			floorObject->SetName(L"Floor");
			Transform* floorTr = floorObject->GetTransform();
			floorTr->SetScale(Vector3(19.180f * 4.0f, 1.920f * 4.0f, FrontLayerZ));
			floorTr->SetPosition(Vector3(0.0f, -1.920f, ObjectZ));
		}
		// land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, -4.350f, 0.0f), Vector3(62.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right rimit
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(30.50f, 2.950f, 0.0f), Vector3(1.0f, 14.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left rimit
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-30.50f, 2.950f, 0.0f), Vector3(1.0f, 14.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// first
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-26.60f, 1.40f, 0.0f), Vector3(8.50f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		
		// second
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-0.350f, 0.150f, 0.0f), Vector3(17.30f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// second left
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-10.80f, -1.830f, 0.0f), Vector3(6.0f, 1.0f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, 0.7850f);
			landObject->SetName(L"LandObj");
		}
		// second right
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(10.10f, -1.850f, 0.0f), Vector3(6.0f, 1.0f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, -0.7850f);

			landObject->SetName(L"LandObj");
		}


		// third
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(25.70f, 1.40f, 0.0f), Vector3(8.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
	}
	void Scene_Town::addUIObjects()
	{
	}
	void Scene_Town::addGameObjects()
	{
		
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(1.50f, 0.0f, ObjectZ));
		}

		
	}
}
