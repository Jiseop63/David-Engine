#include "daScene_Dungeon2F_Stage2.h"
// 임시
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
		addBackgroundObjects();
		addGameObjects();
		GameDataManager::InitializeMonsterCount(eDungeonScene::F2Stage2, 1);
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
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);
		// Camera 세팅
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.210f, 2.240f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));

		CollisionManager::SetLayer(enums::eLayerType::PlayableProjectile, enums::eLayerType::Boss);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::BossProjectile);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Boss);
		CollisionManager::SetLayer(enums::eLayerType::Boss, enums::eLayerType::Land);

		GameDataManager::EnterMonsterCount(eDungeonScene::F2Stage2, mPortals);

		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Play(Resources::Find<AudioClip>(L"JailBoss"), 30.0f, true);
	}
	void Scene_Dungeon2F_Stage2::OnExit()
	{
		SceneManager::GetPlayerScript()->CreatureIsNotGround();
		GameDataManager::ExitMonsterCount(eDungeonScene::F2Stage2);
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
	}
	void Scene_Dungeon2F_Stage2::addBackgroundObjects()
	{
		// Stage : 352 304
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"2FStage2Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.520f * 4.0f, 3.040f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
		}

		// Ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, -5.950f, 0.0f), Vector3(14.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-7.40f, -3.950f, 0.0f), Vector3(1.0f, 3.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// LeftTop
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-6.50f, 1.350f, 0.0f), Vector3(1.50f, 8.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(7.40f, -3.950f, 0.0f), Vector3(1.0f, 3.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// RightTop
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(6.50f, 1.350f, 0.0f), Vector3(1.50f, 8.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, 5.90f, 0.0f), Vector3(12.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// 1F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-0.020f, -4.350f, 0.0f), Vector3(5.150f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 2F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-3.550f, -2.450f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 2F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(3.550f, -2.450f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 3F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-4.150f, -0.550f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 3F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(4.150f, -0.550f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 4F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-4.150f, 1.350f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// 4F Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(4.150f, 1.350f, 0.0f), Vector3(1.90f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// rooftop Platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(0.0f, 2.650f, 0.0f), Vector3(3.80f, 0.450f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}

	}
	void Scene_Dungeon2F_Stage2::addGameObjects()
	{
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetPosition(math::Vector3(6.50f, -4.20f, 0.0f));
			portalScript->SetSceneName(L"Scene_Dungeon2F_Stage1");
			portalScript->SetExitPosition(math::Vector3(6.0f, -1.0f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetPosition(math::Vector3(-6.50f, -4.20f, 0.0f));
			portalScript->SetSceneName(L"Scene_Town");
			portalScript->SetExitPosition(math::Vector3(-12.0f, -1.50f, 0.0f));
			portalScript->SetRotate(1.570f);
			mPortals.push_back(portalScript);
		}
		// skell Boss
		{
			SkellBossScript* bossScript = objects::InstantiateSkellBoss(this);
			bossScript->GetOwner()->GetTransform()->SetPosition(Vector3(0.0f, -0.50f, ObjectZ));
		}
	}
}