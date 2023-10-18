#include "daScene_dungeon1_4.h"
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
	Scene_dungeon1_4::Scene_dungeon1_4()
	{
	}
	Scene_dungeon1_4::~Scene_dungeon1_4()
	{
	}
	void Scene_dungeon1_4::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon1_4, 6);
	}
	void Scene_dungeon1_4::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon1_4::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon1_4::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon1_4::OnEnter()
	{
		// player ¼¼ÆÃ
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);

		// Camera ¼¼ÆÃ
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(2.770f, 1.920f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));

		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon1_4, mPortals);
	}
	void Scene_dungeon1_4::OnExit()
	{
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon1_4);
		SceneManager::GetPlayerScript()->ClearCreature();
	}
	void Scene_dungeon1_4::addBackgroundObjects()
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
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon1-4Material");
			stageObject->GetTransform()->SetScale(math::Vector3(19.20f, 11.520f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(4.50f, -5.0f, 0.0f), math::Vector3(14.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-9.20f, 0.750f, 0.0f), math::Vector3(2.0f, 1.0f, 1.0f));

			landObject->SetName(L"LandObj");
		}
		// left rotate
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-5.60f, -2.10f, 0.0f), math::Vector3(8.50f, 1.0f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, -0.7850f));

			landObject->SetName(L"LandObj");
		}
		// left top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-9.250f, 4.750f, 0.0f), math::Vector3(2.0f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, 6.050f, 0.0f), math::Vector3(18.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(8.750f, 2.0f, 0.0f), math::Vector3(1.0f, 8.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(8.750f, 2.0f, 0.0f), math::Vector3(1.0f, 8.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// top platform 
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-4.650f, 2.250f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(4.50f, 2.250f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		// bottom platform
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 0.0f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}
		{
			GameObject* platformObject = objects::InstantiatePlatformObject(
				this, math::Vector3(6.40f, 0.20f, 0.0f), math::Vector3(2.50f, 0.40f, 1.0f));
			platformObject->SetName(L"platformObj");
		}

		// Æ÷Å» °´Ã¼
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_3");
			portalScript->SetPosition(math::Vector3(-9.0f, 2.60f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(7.0f, -1.750f, 0.0f));
			portalScript->SetRotate(-1.570f);

			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_5");
			portalScript->SetPosition(math::Vector3(9.0f, -3.20f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-4.50f, -3.750f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
	}
	void Scene_dungeon1_4::addGameObjects()
	{
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(1.50f, 0.0f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.750f, 3.50f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-0.30f, 1.0f, ObjectZ));
		}

		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(4.0f, -3.50f, ObjectZ));
		}
		// test enemy
		{
			BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
			bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(5.50f, 4.0f, ObjectZ));
		}
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.0f, 0.20f, ObjectZ));
		}


		{
			FairyScript* fairyScript = objects::InstantiateFairy(this);
			fairyScript->SetPosition(6.50f, 2.0f);
			mFairySript = fairyScript;
		}
	}
}