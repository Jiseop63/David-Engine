#include "daScene_Dungeon1F_Stage2.h"
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
	Scene_Dungeon1F_Stage2::Scene_Dungeon1F_Stage2()
	{
	}
	Scene_Dungeon1F_Stage2::~Scene_Dungeon1F_Stage2()
	{
	}
	void Scene_Dungeon1F_Stage2::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
	}
	void Scene_Dungeon1F_Stage2::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F_Stage3");
		}
		Scene::Update();
	}
	void Scene_Dungeon1F_Stage2::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon1F_Stage2::Render()
	{
		Scene::Render();
	}
	void Scene_Dungeon1F_Stage2::OnEnter()
	{
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->SetPlayerPosition(math::Vector3(-7.0f, -4.0f, ObjectZ));
		player->SetPlayerVelocity(math::Vector2::Zero);
		player->ChangeState(ePlayerState::Idle);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(true);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(2.750f, 1.850f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		
	}
	void Scene_Dungeon1F_Stage2::OnExit()
	{
		SceneManager::GetPlayerScript()->PlayerIsNotGround();
	}
	void Scene_Dungeon1F_Stage2::addBackgroundObjects()
	{
		// Stage : 480 288
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"1FStage2Material");
			stageObject->GetTransform()->SetScale(math::Vector3(4.80f * 4.0f, 2.880f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-2.2250f, -3.02750f, 0.0f), Vector3(2.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// ground top left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-3.6070f, -3.6050f, 0.0f), Vector3(2.0f, 1.0f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, 0.7850f));
			landObject->SetName(L"LandObj");
		}
		// ground top right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-0.8470f, -3.6050f, 0.0f), Vector3(2.0f, 1.0f, 1.0f));
			landObject->GetTransform()->SetRotation(math::Vector3(0.0f, 0.0f, -0.7850f));
			landObject->SetName(L"LandObj");
		}

		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-0.50f, -5.0f, 0.0f), Vector3(18.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-9.30f, -4.350f, 0.0f), Vector3(2.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-9.30f, 1.950f, 0.0f), Vector3(2.0f, 6.50f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(8.80f, 0.0f, 0.0f), Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// center
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-0.650f, 0.30f, 0.0f), Vector3(10.20f, 0.60f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 1f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-6.40f, -2.70f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 2f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-7.050f, -0.050f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform left 3f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-7.050f, -1.40f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 1f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(5.150f, -2.70f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 2f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(5.80f, -0.050f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// platform right 3f 
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(5.80f, -1.40f, 0.0f), Vector3(1.30f, 0.250f, 1.0f));
			landObject->SetName(L"LandObj");
		}
	}
	void Scene_Dungeon1F_Stage2::addGameObjects()
	{
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(3.50f, 2.0f, ObjectZ));
		}
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(1.50f, 2.0f, ObjectZ));
		}
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(-1.50f, 2.0f, ObjectZ));
		}
	}
}