#include "daScene_dungeon2_1.h"
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
	Scene_dungeon2_1::Scene_dungeon2_1()
	{
	}
	Scene_dungeon2_1::~Scene_dungeon2_1()
	{
	}
	void Scene_dungeon2_1::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
	}
	void Scene_dungeon2_1::Update()
	{
		Scene::Update();
	}
	void Scene_dungeon2_1::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_dungeon2_1::Render()
	{
		Scene::Render();
	}
	void Scene_dungeon2_1::OnEnter()
	{
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);
		// Camera 세팅
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		GameDataManager::SetCameraMovableRange(math::Vector2(0.530f, 0.320f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
		// 충돌 세팅
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::MonsterProjectile);
		CollisionManager::SetLayer(enums::eLayerType::PlayableProjectile, enums::eLayerType::Monster);
		// 던전 포탈 세팅


	}
	void Scene_dungeon2_1::OnExit()
	{
		SceneManager::GetPlayerScript()->ClearCreature();
	}
	void Scene_dungeon2_1::addBackgroundObjects()
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
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"Dungeon2-1Material");
			stageObject->GetTransform()->SetScale(math::Vector3(14.720f, 8.320f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// Close Door : 57, 65
		{
			GameObject* doorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::ENV, L"Close1FMaterial");
			doorObject->GetTransform()->SetScale(math::Vector3(0.570f * 4.0f, 0.650f * 4.0f, 1.0f));
			doorObject->GetTransform()->SetPosition(math::Vector3(-3.0f, -1.0f + 0.080f, ObjectZ + 0.00010f));
		}
		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(1.0f, -2.750f, 0.0f), math::Vector3(13.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(-6.50f, 0.0f, 0.0f), math::Vector3(1.0f, 5.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(0.30f, 3.0f, 0.0f), math::Vector3(13.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// right top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, math::Vector3(7.50f, 1.50f, 0.0f), math::Vector3(1.50f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

	}

	void Scene_dungeon2_1::addGameObjects()
	{
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_dungeon2_boss");
			portalScript->SetPosition(math::Vector3(7.150f, -1.0f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-5.0f, -4.250f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
	}
}
