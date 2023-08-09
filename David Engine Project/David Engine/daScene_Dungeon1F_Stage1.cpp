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
			SceneManager::LoadScene(L"Scene_Dungeon2F_Stage1");
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
		// Camera 세팅
		SceneManager::GetMainCameraScript()->GetOwner()->GetComponent<Transform>()->SetPosition(math::Vector3::Zero);
		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->SetPlayerPosition(math::Vector3::Zero);
		player->SetPlayerVelocity(math::Vector2::Zero);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(true);
		// light 세팅
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.50f, 0.50f, 0.50f, 1.0f));
		// 충돌 세팅
		CollisionManager::SetLayer(enums::eLayerType::PlayableAttackCollider, enums::eLayerType::Creature);
	}
	void Scene_Dungeon1F_Stage1::OnExit()
	{
		SceneManager::GetPlayerScript()->PlayerIsNotGround();
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
	}
	void Scene_Dungeon1F_Stage1::addGameObjects()
	{
		// Stage : 384 208
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"1FStage1Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.840f * 4.0f, 2.080f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
		}

		// Close Door : 57, 65
		{
			GameObject* doorObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::ENV, L"Close1FMaterial");
			doorObject->GetTransform()->SetScale(math::Vector3(0.570f * 4.0f, 0.650f * 4.0f, 1.0f));
			doorObject->GetTransform()->SetPosition(math::Vector3(-3.0f, -1.0f + 0.080f, ObjectZ));
		}

		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(1.50f, 0.0f, ObjectZ));
		}
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(2.50f, 0.0f, ObjectZ));
		}
		// test enemy
		{
			SkelScript* skelScript = objects::InstantiateCreature<SkelScript>(this);
			skelScript->GetOwner()->GetTransform()->SetPosition(Vector3(-0.50f, 0.0f, ObjectZ));
		}
		//// test enemy
		//{
		//	GameObject* monsterObject = objects::InstantiateCreature<GameObject>(this, L"AnimationMaterial");
		//	monsterObject->SetName(L"monster");
		//	monsterObject->GetTransform()->SetPosition(Vector3(1.50f, 0.0f, ObjectZ));
		//	monsterObject->AddComponent<CreatureScript>();
		//}

		// land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-1.0f, -2.50f, 0.0f), Vector3(10.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}		
	}
}
