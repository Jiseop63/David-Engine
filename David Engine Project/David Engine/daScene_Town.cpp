#include "daScene_Town.h"

#include "daRenderer.h"
// �ӽ�
#include "daInput.h"
#include "daApplication.h"
#include "daResources.h"
#include "daTexture.h"

#include "daObjects.h"
#include "daGameDataManager.h"

// UI, Object, Components and Camera
#include "daObjectsFastIncludeHeader.h"

extern da::Application application;

namespace da
{
	Scene_Town::Scene_Town()
	{
	}
	Scene_Town::~Scene_Town()
	{
	}
	void Scene_Town::Initialize()
	{
		addBackgroundObject();
		addGameObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F_Stage1");
		}
		Scene::Update();
	}
	void Scene_Town::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Town::Render()
	{
		Scene::Render();
	}
	
	void Scene_Town::OnEnter()
	{
		// Camera ����
		SceneManager::GetMainCameraScript()->GetOwner()->GetComponent<Transform>()->SetPosition(math::Vector3::Zero);
		GameDataManager::SetCameraMovableRange(math::Vector2(38.360f, 3.840f));
		GameDataManager::SetCameraMovaPosition(math::Vector2::Zero);

		// player ����
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->SetPlayerPosition(math::Vector3::Zero);
		player->SetPlayerVelocity(math::Vector2::Zero);
		player->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		player->IsPlayerInDungeon(false);
		// UI ����
		structs::sPlayerStat playerStat = GameDataManager::GetPlayerStat();
		SceneManager::GetLifebarScript()->SetValue(playerStat.MaxHP, playerStat.CurHP);
		SceneManager::GetHUDObject()->SetObjectStates(GameObject::eObjectState::Active);
		SceneManager::GetCursourScrip()->IsIngame(true);

		// �浹 ����
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Playable);
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Creature);
	}
	void Scene_Town::OnExit()
	{
		SceneManager::GetPlayerScript()->PlayerIsNotGround();
	}

	void Scene_Town::addBackgroundObject()
	{
		CameraObject* mainCameraObject = dynamic_cast<CameraObject*>(SceneManager::GetMainCameraScript()->GetOwner());
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, BackgroundZ));
			backGround->SetName(L"sky");
		}
		// forest Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -2.0f, BackLayerZ));
			backGround->SetName(L"forest");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"ù��° ���̾� ��ũ��Ʈ");
		}
		// trees Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -4.0f, FrontLayerZ));
			backGround->SetName(L"trees");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"�ι�° ���̾� ��ũ��Ʈ");
		}
	}
	void Scene_Town::addUIObjects()
	{
	}
	void Scene_Town::addGameObjects()
	{
		// floor
		{
			// 694 128
			GameObject* floorObject = objects::InstantiateGameObject<GameObject>(this, enums::eLayerType::FrontBackGround, L"TownMapTileMaterial");
			floorObject->SetName(L"Floor");
			Transform* floorTr = floorObject->GetTransform();
			floorTr->SetScale(Vector3(19.180f * 4.0f, 1.920f * 4.0f, 1.0f));
			floorTr->SetPosition(Vector3(0.0f, -4.0f, ObjectZ));
		}

		//// test enemy
		//{
		//	GameObject* monsterObject = objects::InstantiateCreature<GameObject>(this, L"AnimationMaterial");
		//	monsterObject->SetName(L"monster");
		//	monsterObject->GetTransform()->SetPosition( Vector3(1.50f, 0.0f, ObjectZ) );
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
