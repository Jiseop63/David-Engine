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
		player->SetCreaturePosition(math::Vector3::Zero);
		player->SetCreatureVelocity(math::Vector2::Zero);
		player->ChangeState(ePlayerState::Idle);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(true);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(0.210f, 2.240f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.40f, 0.40f, 0.40f, 1.0f));

		CollisionManager::SetLayer(enums::eLayerType::PlayableProjectile, enums::eLayerType::Boss);

	}
	void Scene_Dungeon2F_Stage2::OnExit()
	{
		SceneManager::GetPlayerScript()->CreatureIsNotGround();
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
		//
		//// test enemy
		//{
		//	SkelScript* skelScript = objects::InstantiateSkel(this);
		//	skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(-0.50f, 0.0f, ObjectZ));
		//}

		// skell Boss
		{
			SkellBossScript* bossScript = objects::InstantiateSkellBoss(this);
			bossScript->GetOwner()->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, ObjectZ));
		}
	}
}