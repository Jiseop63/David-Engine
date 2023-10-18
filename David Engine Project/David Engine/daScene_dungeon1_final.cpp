#include "daScene_dungeon1_final.h"
#include "daRenderer.h"

#include "daGameDataManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"


// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"

namespace da
{
	Scene_dungeon1_final::Scene_dungeon1_final()
	{
	}
	Scene_dungeon1_final::~Scene_dungeon1_final()
	{
	}
	void Scene_dungeon1_final::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon1_final, 1);

	}
	void Scene_dungeon1_final::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon1_final::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon1_final::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon1_final::OnEnter()
	{
		// player ¼¼ÆÃ
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);

		// Camera ¼¼ÆÃ
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.0f, 1.280f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));

		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon1_final, mPortals);


	}
	void Scene_dungeon1_final::OnExit()
	{		
		SceneManager::GetPlayerScript()->ClearCreature();
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon1_final);
	}
	void Scene_dungeon1_final::addBackgroundObjects()
	{
		// Stage : 288 256
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon1-finalMaterial");
			stageObject->GetTransform()->SetScale(math::Vector3(11.520f, 10.240f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-3.40f, -4.350f, 0.0f), math::Vector3(4.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(3.40f, -4.350f, 0.0f), math::Vector3(4.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-5.650f, 0.0f, 0.0f), math::Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(5.650f, 0.0f, 0.0f), math::Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, 5.050f, 0.0f), math::Vector3(12.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// left platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-2.850f, -2.0f, 0.0f), math::Vector3(1.750f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(2.850f, -2.0f, 0.0f), math::Vector3(1.750f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// lefttop platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-3.50f, 0.550f, 0.0f), math::Vector3(3.30f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// righttop platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(3.50f, 0.550f, 0.0f), math::Vector3(3.30f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// center land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, 0.380f, 0.0f), math::Vector3(3.80f, 0.60f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// Æ÷Å» °´Ã¼
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetPosition(math::Vector3(0.0f, -4.350f, 0.0f));
			portalScript->SetSceneName(L"Scene_dungeon1_3");
			portalScript->SetExitPosition(math::Vector3(0.0f, 4.250f, 0.0f));
			mPortals.push_back(portalScript);
		}
		{
			BossPortalScript* portalScript = objects::InstantiateBossPortal(this);
			portalScript->SetPosition(math::Vector3(0.0f, 2.0f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-3.0f, -1.250f, 0.0f));
			portalScript->SetSceneName(L"Scene_dungeon2_1");
			mPortals.push_back(portalScript);
		}
	}
	void Scene_dungeon1_final::addGameObjects()
	{
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(1.50f, 1.250f, ObjectZ));
		}

		//// test enemy
		//{
		//	SkelScript* skelScript = objects::InstantiateSkel(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-1.50f, 1.250f, ObjectZ));
		//}

		//// test enemy
		//{
		//	BansheeScript* skelScript = objects::InstantiateBanshee(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(2.750f, -2.50f, ObjectZ));
		//}
		//// test enemy
		//{
		//	BansheeScript* skelScript = objects::InstantiateBanshee(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-2.750f, -2.50f, ObjectZ));
		//}

		//// test enemy
		//{
		//	BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
		//	bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-3.50f, 2.80f, ObjectZ));
		//}

		//// test enemy
		//{
		//	BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
		//	bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(3.50f, 2.80f, ObjectZ));
		//}

		{
			FairyScript* fairyScript = objects::InstantiateFairy(this);
			fairyScript->SetPosition(2.0f, -1.0f);
			mFairySript = fairyScript;
		}
	}
}