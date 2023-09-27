#include "daScene_Dungeon1F_Stage1.h"
#include "daRenderer.h"

// 임시
#include "daInput.h"
#include "daGameDataManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"


// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"
namespace da
{
	Scene_Dungeon1F_Stage1::Scene_Dungeon1F_Stage1()
	{
	}
	Scene_Dungeon1F_Stage1::~Scene_Dungeon1F_Stage1()
	{
	}
	void Scene_Dungeon1F_Stage1::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
	}
	void Scene_Dungeon1F_Stage1::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F_Stage2");
		}
		Scene::Update();
	}
	void Scene_Dungeon1F_Stage1::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon1F_Stage1::Render()
	{
		Scene::Render();
	}
	void Scene_Dungeon1F_Stage1::OnEnter()
	{
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->ClearCreature();
		player->SetCreaturePosition(math::Vector3(-3.0f, -1.0f, ObjectZ));
		player->IsPlayerInDungeon(true);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(0.80f, 0.0f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));
		// 충돌 세팅
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::MonsterProjectile);
		CollisionManager::SetLayer(enums::eLayerType::PlayableProjectile, enums::eLayerType::Monster);
	}
	void Scene_Dungeon1F_Stage1::OnExit()
	{
		SceneManager::GetPlayerScript()->CreatureIsNotGround();
	}
	void Scene_Dungeon1F_Stage1::addBackgroundObjects()
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
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"1FStage1Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.840f * 4.0f, 2.080f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// Close Door : 57, 65
		{
			GameObject* doorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::ENV, L"Close1FMaterial");
			doorObject->GetTransform()->SetScale(math::Vector3(0.570f * 4.0f, 0.650f * 4.0f, 1.0f));
			doorObject->GetTransform()->SetPosition(math::Vector3(-3.0f, -1.0f + 0.080f, FrontLayerZ));
		}
		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.30f, -2.750f, 0.0f), Vector3(12.40f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-6.250f, -0.50f, 0.0f), Vector3(1.0f, 5.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(7.150f, -2.60f, 0.0f), Vector3(1.50f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// Top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.30f, 2.050f, 0.0f), Vector3(12.40f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(7.150f, 1.950f, 0.0f), Vector3(1.50f, 2.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
	}
	void Scene_Dungeon1F_Stage1::addGameObjects()
	{
		
	}
}
