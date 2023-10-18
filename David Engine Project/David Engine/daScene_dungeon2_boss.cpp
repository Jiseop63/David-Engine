#include "daScene_dungeon2_boss.h"
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
	Scene_dungeon2_boss::Scene_dungeon2_boss()
	{
	}
	Scene_dungeon2_boss::~Scene_dungeon2_boss()
	{
	}
	void Scene_dungeon2_boss::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::dungeon2_boss, 1);
	}
	void Scene_dungeon2_boss::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon2_boss::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon2_boss::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon2_boss::OnEnter()
	{
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);
		// Camera 세팅
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.210f, 2.240f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
		// 충돌 세팅

		CollisionManager::SetLayer(enums::eLayerType::PlayableProjectile, enums::eLayerType::Boss);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::BossProjectile);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Boss);
		CollisionManager::SetLayer(enums::eLayerType::Boss, enums::eLayerType::Land);
		// 던전 포탈 세팅
		GameDataManager::EnterMonsterCount(eDungeonScene::dungeon2_boss, mPortals);


		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Play(Resources::Find<AudioClip>(L"JailBoss"), 30.0f, true);
	}
	void Scene_dungeon2_boss::OnExit()
	{
		GameDataManager::ExitMonsterCount(eDungeonScene::dungeon2_boss);
		SceneManager::GetPlayerScript()->ClearCreature();
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
	}
	void Scene_dungeon2_boss::addBackgroundObjects()
	{
		// SubBG : 타일에 가려짐
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"DungeonSubBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
		}
		// Stage : 384 208
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon2-bossMaterial");
			stageObject->GetTransform()->SetScale(math::Vector3(14.080f, 12.160f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// Ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, -5.950f, 0.0f), math::Vector3(14.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-7.40f, -3.950f, 0.0f), math::Vector3(1.0f, 3.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// LeftTop
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-6.50f, 1.350f, 0.0f), math::Vector3(1.50f, 8.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(7.40f, -3.950f, 0.0f), math::Vector3(1.0f, 3.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// RightTop
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(6.50f, 1.350f, 0.0f), math::Vector3(1.50f, 8.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.0f, 5.90f, 0.0f), math::Vector3(12.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// 1F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-0.020f, -4.350f, 0.0f), math::Vector3(5.150f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 2F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-3.550f, -2.450f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 2F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(3.550f, -2.450f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 3F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-4.150f, -0.550f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 3F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(4.150f, -0.550f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 4F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(-4.150f, 1.350f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 4F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(4.150f, 1.350f, 0.0f), math::Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// rooftop Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, math::Vector3(0.0f, 2.650f, 0.0f), math::Vector3(3.80f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
	}

	void Scene_dungeon2_boss::addGameObjects()
	{
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetPosition(math::Vector3(-6.50f, -4.20f, 0.0f));
			portalScript->SetSceneName(L"Scene_dungeon2_1");
			portalScript->SetExitPosition(math::Vector3(-12.0f, -1.50f, 0.0f));
			portalScript->SetRotate(1.570f);
			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetPosition(math::Vector3(6.50f, -4.20f, 0.0f));
			portalScript->SetSceneName(L"Scene_Title");
			portalScript->SetExitPosition(math::Vector3(6.0f, -1.0f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
		// skell Boss
		{
			SkellBossScript* bossScript = objects::InstantiateSkellBoss(this);
			bossScript->GetOwner()->GetTransform()->SetPosition(math::Vector3(0.0f, -0.50f, ObjectZ));
		}

	}
}
