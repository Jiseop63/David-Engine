#include "daScene_dungeon1_2.h"
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
	Scene_dungeon1_2::Scene_dungeon1_2()
	{
	}
	Scene_dungeon1_2::~Scene_dungeon1_2()
	{
	}
	void Scene_dungeon1_2::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon1_2, 1);
	}
	void Scene_dungeon1_2::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon1_2::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon1_2::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon1_2::OnEnter()
	{
		// player ¼¼ÆÃ
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);

		// Camera ¼¼ÆÃ
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(2.770f, 1.920f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));

		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon1_2, mPortals);
	}
	void Scene_dungeon1_2::OnExit()
	{
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon1_2);
		SceneManager::GetPlayerScript()->ClearCreature();
	}
	void Scene_dungeon1_2::addBackgroundObjects()
	{
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"DungeonSubBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
		}
		// Stage : 480 288
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon1-2Material");
			stageObject->GetTransform()->SetScale(math::Vector3(19.20f, 11.520f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// top left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-4.80f, 5.0f, 0.0f), math::Vector3(7.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(4.80f, 5.0f, 0.0f), math::Vector3(7.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-0.50f, -5.0f, 0.0f), math::Vector3(18.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-2.2250f, -2.70f, 0.0f), math::Vector3(2.0f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground top left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-3.90f, -3.350f, 0.0f), math::Vector3(2.0f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, 0.7850f));
			landObject->SetName(L"LandObj");
		}
		// ground top right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-0.550f, -3.350f, 0.0f), math::Vector3(2.0f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, -0.7850f));
			landObject->SetName(L"LandObj");
		}
		// ground middle left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-4.850f, -4.0f, 0.0f), math::Vector3(0.80f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, 0.0f));
			landObject->SetName(L"LandObj");
		}
		// ground middle right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.350f, -4.0f, 0.0f), math::Vector3(0.80f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, 0.0f));
			landObject->SetName(L"LandObj");
		}
		// ground bottom left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-5.50f, -4.30f, 0.0f), math::Vector3(1.0f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, 0.7850f));
			landObject->SetName(L"LandObj");
		}
		// ground bottom right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(1.050f, -4.30f, 0.0f), math::Vector3(1.0f, 0.20f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, -0.7850f));
			landObject->SetName(L"LandObj");
		}

		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-9.30f, -4.350f, 0.0f), math::Vector3(2.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-9.30f, 1.950f, 0.0f), math::Vector3(2.0f, 6.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(8.80f, 0.0f, 0.0f), math::Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// center
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-0.650f, 0.30f, 0.0f), math::Vector3(10.20f, 0.60f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 1f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-6.40f, -2.70f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 2f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-7.050f, -0.050f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 3f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-7.050f, -1.40f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 1f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(5.150f, -2.70f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 2f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(5.80f, -0.050f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 3f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(5.80f, -1.40f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 4f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 1.80f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 5f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 3.0f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 6f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 4.40f, 0.0f), math::Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}


		// Æ÷Å» °´Ã¼
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_1");
			portalScript->SetPosition(math::Vector3(-9.30f, -2.50f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(6.0f, 0.0f, 0.0f));
			portalScript->SetRotate(1.570f);

			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon1_3");
			portalScript->SetPosition(math::Vector3(0.0f, 5.0f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-1.0f, -2.0f, 0.0f));
			mPortals.push_back(portalScript);
		}
	}
	void Scene_dungeon1_2::addGameObjects()
	{
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateSkel(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(1.50f, 0.0f, ObjectZ));
		}

		//// test enemy
		//{
		//	SkelScript* skelScript = objects::InstantiateSkel(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-1.50f, 0.0f, ObjectZ));
		//}
		//// test enemy
		//{
		//	SkelScript* skelScript = objects::InstantiateSkel(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-1.50f, 2.0f, ObjectZ));
		//}

		//// test enemy
		//{
		//	BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
		//	bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(-5.50f, 2.50f, ObjectZ));
		//}

		//// test enemy
		//{
		//	BansheeScript* bansheeScript = objects::InstantiateBanshee(this);
		//	bansheeScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(5.50f, 2.50f, ObjectZ));
		//}

		{
			FairyScript* fairyScript = objects::InstantiateFairy(this);
			fairyScript->SetPosition(-1.50f, 2.0f);
			mFairySript = fairyScript;
		}
	}
}