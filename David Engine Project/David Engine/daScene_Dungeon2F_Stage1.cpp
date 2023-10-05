#include "daScene_Dungeon2F_Stage1.h"
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
	Scene_Dungeon2F_Stage1::Scene_Dungeon2F_Stage1()
	{
	}
	Scene_Dungeon2F_Stage1::~Scene_Dungeon2F_Stage1()
	{
	}
	void Scene_Dungeon2F_Stage1::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
	}
	void Scene_Dungeon2F_Stage1::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon2F_Stage2");
		}
		Scene::Update();
	}
	void Scene_Dungeon2F_Stage1::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon2F_Stage1::Render()
	{
		Scene::Render();
	}
	void Scene_Dungeon2F_Stage1::OnEnter()
	{
		
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->IsPlayerInDungeon(true);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(0.5250f, 0.1750f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
	}
	void Scene_Dungeon2F_Stage1::OnExit()
	{
		SceneManager::GetPlayerScript()->CreatureIsNotGround();
	}
	void Scene_Dungeon2F_Stage1::addBackgroundObjects()
	{
		// Stage : 336 208
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"2FStage1Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.680f * 4.0f, 2.080f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
		}

		// Close Door : 57, 65
		{
			GameObject* doorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::ENV, L"Close1FMaterial");
			doorObject->GetTransform()->SetScale(math::Vector3(0.570f * 4.0f, 0.650f * 4.0f, 1.0f));
			doorObject->GetTransform()->SetPosition(math::Vector3(-3.0f, -1.0f + 0.080f, ObjectZ));
		}

		// bottom land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.80f, -2.750f, 0.0f), Vector3(14.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.40f, 2.750f, 0.0f), Vector3(13.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// left land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-6.550f, 0.0f, 0.0f), Vector3(1.0f, 4.60f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// right land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(7.250f, 1.30f, 0.0f), Vector3(1.0f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
	}
	void Scene_Dungeon2F_Stage1::addGameObjects()
	{
		{
			DungeonPortalScript* portalScript = objects::InstantiateDungeonPortal(this);
			portalScript->SetSceneName(L"Scene_Dungeon2F_Stage2");
			portalScript->SetPosition(math::Vector3(7.0f, -1.0f, 0.0f));
			portalScript->SetExitPosition(math::Vector3(-5.250f, -4.20f, 0.0f));
			portalScript->SetRotate(-1.570f);
			mPortals.push_back(portalScript);
		}
	}
}