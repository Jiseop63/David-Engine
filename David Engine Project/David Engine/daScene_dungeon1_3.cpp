#include "daScene_dungeon1_3.h"
#include "daRenderer.h"

// ÀÓ½Ã
#include "daGameDataManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"

// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"
namespace da
{
	Scene_dungeon1_3::Scene_dungeon1_3()
	{
	}
	Scene_dungeon1_3::~Scene_dungeon1_3()
	{
	}
	void Scene_dungeon1_3::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon1_3, 4);
	}
	void Scene_dungeon1_3::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon1_3::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon1_3::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon1_3::OnEnter()
	{
		// player ¼¼ÆÃ
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);

		// Camera ¼¼ÆÃ
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.850f, 0.320f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));

		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon1_3, mPortals);
	}
	void Scene_dungeon1_3::OnExit()
	{
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon1_3);
		SceneManager::GetPlayerScript()->ClearCreature();
	}
	void Scene_dungeon1_3::addBackgroundObjects()
	{
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"DungeonSubBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
		}
		// Stage : 384 208
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon1-3Material");
			stageObject->GetTransform()->SetScale(math::Vector3(15.360f, 8.320f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-3.50f, -3.40f, 0.0f), math::Vector3(7.40f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(4.80f, -3.40f, 0.0f), math::Vector3(4.50f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-7.0f, 0.0f, 0.0f), math::Vector3(1.0f, 6.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right bottom
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(7.650f, -2.60f, 0.0f), math::Vector3(1.50f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(7.650f, 1.950f, 0.0f), math::Vector3(1.50f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-3.50f, 3.40f, 0.0f), math::Vector3(7.40f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(4.80f, 3.40f, 0.0f), math::Vector3(4.50f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}


		// center
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, -0.750f, 0.0f), math::Vector3(7.80f, 0.750f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform 1
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(1.250f, 0.70f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// platform 2s
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(1.250f, 2.10f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}

		// Æ÷Å» °´Ã¼
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_2");
			portalScript->SetPosition(math::Vector3(1.30f, -3.750f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(0.0f, 4.750f, 0.0f));

			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_4");
			portalScript->SetPosition(math::Vector3(7.250f, -0.350f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-8.0f, 2.50f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_final");
			portalScript->SetPosition(math::Vector3(1.30f, 3.80f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(0.0f, -3.0f, 0.0f));
			mPortals.push_back(portalScript);
		}
	}
	void Scene_dungeon1_3::addGameObjects()
	{
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(1.50f, 0.20f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* skelScript = objects::InstantiateBanshee(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-4.750f, 1.50f, ObjectZ));
		}

		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.0f, 0.20f, ObjectZ));
		}

		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-5.0f, - 2.20f, ObjectZ));
		}


		{
			FairyScript* fairyScript = objects::InstantiateFairy(this);
			fairyScript->SetPosition(-1.50f, 0.750f);
			mFairySript = fairyScript;
		}
	}
}