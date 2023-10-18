#include "daScene_dungeon1_5.h"
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
	Scene_dungeon1_5::Scene_dungeon1_5()
	{
	}
	Scene_dungeon1_5::~Scene_dungeon1_5()
	{
	}
	void Scene_dungeon1_5::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon1_5, 5);
	}
	void Scene_dungeon1_5::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon1_5::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon1_5::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon1_5::OnEnter()
	{
		// player ¼¼ÆÃ
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);

		// Camera ¼¼ÆÃ
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.210f, 1.920f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));

		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon1_5, mPortals);
	}
	void Scene_dungeon1_5::OnExit()
	{
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon1_5);
		SceneManager::GetPlayerScript()->ClearCreature();
	}
	void Scene_dungeon1_5::addBackgroundObjects()
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
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon1-5Material");
			stageObject->GetTransform()->SetScale(math::Vector3(14.080f, 11.520f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, -5.10f, 0.0f), math::Vector3(10.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-4.80f, 2.50f, 0.0f), math::Vector3(2.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(4.30f, 0.0f, 0.0f), math::Vector3(1.0f, 12.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, 6.00f, 0.0f), math::Vector3(8.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// platfomr 1
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-2.550f, -2.450f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// platfomr 2
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, -1.350f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// platfomr 3
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(2.50f, -0.20f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// platfomr 4
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 0.90f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// Æ÷Å» °´Ã¼
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_4");
			portalScript->SetPosition(math::Vector3(-5.0f, -3.50f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(8.750f, -3.0f, 0.0f));
			portalScript->SetRotate(-1.570f);

			mPortals.push_back(portalScript);
		}
	}
	void Scene_dungeon1_5::addGameObjects()
	{
		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.30f, -1.0f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(2.40f, 1.20f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(2.40f, -3.20f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.40f, 2.0f, ObjectZ));
		}


		{
			FairyScript* fairyScript = objects::InstantiateFairy(this);
			fairyScript->SetPosition(0.0f, 2.0f);
			mFairySript = fairyScript;
		}
	}
}