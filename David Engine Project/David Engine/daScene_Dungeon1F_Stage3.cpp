#include "daScene_Dungeon1F_Stage3.h"
#include "daRenderer.h"

// �ӽ�
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
	Scene_Dungeon1F_Stage3::Scene_Dungeon1F_Stage3()
	{
	}
	Scene_Dungeon1F_Stage3::~Scene_Dungeon1F_Stage3()
	{
	}
	void Scene_Dungeon1F_Stage3::Initialize()
	{
		addBackgroundObjects();
		addGameObjects();
	}
	void Scene_Dungeon1F_Stage3::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon2F_Stage1");
		}
		Scene::Update();
	}
	void Scene_Dungeon1F_Stage3::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon1F_Stage3::Render()
	{
		Scene::Render();
	}
	void Scene_Dungeon1F_Stage3::OnEnter()
	{
		// player ����
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->SetPlayerPosition(math::Vector3(0.0f, -2.0f, ObjectZ));
		player->SetPlayerVelocity(math::Vector2::Zero);
		player->ChangeState(ePlayerState::Idle);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(true);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();

		// Camera ����
		GameDataManager::SetCameraMovableRange(math::Vector2(0.80f, 6.0f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
	}
	void Scene_Dungeon1F_Stage3::OnExit()
	{
		SceneManager::GetPlayerScript()->PlayerIsNotGround();
	}
	void Scene_Dungeon1F_Stage3::addBackgroundObjects()
	{

		// Close Door : 57, 65
		{
			GameObject* doorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::ENV, L"Close1FMaterial");
			doorObject->GetTransform()->SetScale(math::Vector3(0.570f * 4.0f, 0.650f * 4.0f, 1.0f));
			doorObject->GetTransform()->SetPosition(math::Vector3(0.0f, 1.90f, FrontLayerZ));
		}

		// Stage : 288 256
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"1FStage3Material");
			stageObject->GetTransform()->SetScale(math::Vector3(2.880f * 4.0f, 2.560f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
		}

		// ground
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, -4.350f, 0.0f), Vector3(12.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-5.650f, 0.0f, 0.0f), Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(5.650f, 0.0f, 0.0f), Vector3(1.0f, 10.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// top
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, 5.050f, 0.0f), Vector3(12.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// left platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-2.850f, -2.0f, 0.0f), Vector3(1.750f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(2.850f, -2.0f, 0.0f), Vector3(1.750f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// lefttop platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-3.80f, 0.50f, 0.0f), Vector3(3.30f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// righttop platform
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(3.80f, 0.50f, 0.0f), Vector3(3.30f, 0.20f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// center land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, 0.370f, 0.0f), Vector3(3.80f, 0.60f, 1.0f));
			landObject->SetName(L"LandObj");
		}
	}
	void Scene_Dungeon1F_Stage3::addGameObjects()
	{
	}
}