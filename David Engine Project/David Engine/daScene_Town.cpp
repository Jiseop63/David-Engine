#include "daScene_Town.h"

#include "daRenderer.h"
// 임시
#include "daInput.h"
#include "daApplication.h"
#include "daResources.h"
#include "daTexture.h"

#include "daObjects.h"
#include "daGameDataManager.h"

// UI, Object, Components and Camera
#include "daObjectsFastIncludeHeader.h"
#include "daTownPortalScript.h"

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

		// player 세팅
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->ClearCreature();
		player->SetCreaturePosition(math::Vector3(-12.0f, -1.50f, ObjectZ));
		player->IsPlayerInDungeon(false);
		math::Vector3 playerPos = player->GetOwner()->GetTransform()->GetPosition();
		// Camera 세팅
		GameDataManager::SetCameraMovableRange(math::Vector2(22.0f, 1.80f));
		GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
		// UI 세팅
		SceneManager::GetLifebarScript()->SetValue(&player->GetCreatureStat()->MaxHP, &player->GetCreatureStat()->CurHP);
		SceneManager::GetHUDObject()->SetObjectStates(GameObject::eObjectState::Active);
		SceneManager::GetCursourScrip()->IsIngame(true);
		SceneManager::GetLightObject()->GetComponent<Light>()->SetColor(math::Vector4(0.70f, 0.70f, 0.70f, 1.0f));
		// 충돌 세팅
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Playable);
		CollisionManager::SetLayer(enums::eLayerType::Land, enums::eLayerType::Monster);
		CollisionManager::SetLayer(enums::eLayerType::Platform, enums::eLayerType::Playable);
		CollisionManager::SetLayer(enums::eLayerType::Platform, enums::eLayerType::Monster);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Monster);
		CollisionManager::SetLayer(enums::eLayerType::Playable, enums::eLayerType::Portal);
	}
	void Scene_Town::OnExit()
	{
		SceneManager::GetPlayerScript()->CreatureIsNotGround();
	}

	void Scene_Town::addBackgroundObject()
	{
		CameraObject* mainCameraObject = dynamic_cast<CameraObject*>(SceneManager::GetMainCameraScript()->GetOwner());
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Background, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 1.920f, BackgroundZ));
			backGround->SetName(L"sky");
		}
		// forest Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 1.0f, BackLayerZ));
			backGround->SetName(L"forest");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"첫번째 레이어 스크립트");
		}
		// trees Layer
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(MaxPositionX * 2.0f, MaxPositionY * 2.0f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, FrontLayerZ));
			backGround->SetName(L"trees");
			LayerScript* bgLayerScript = backGround->AddComponent<LayerScript>();
			
			bgLayerScript->SetCamera(mainCameraObject->GetCameraComponent());
			bgLayerScript->SetName(L"두번째 레이어 스크립트");
		}
		// floor
		{
			// 694 128
			GameObject* floorObject = objects::InstantiateGameObject<GameObject>(this, enums::eLayerType::FrontBackGround, L"TownMapTileMaterial");
			floorObject->SetName(L"Floor");
			Transform* floorTr = floorObject->GetTransform();
			floorTr->SetScale(Vector3(19.180f * 4.0f, 1.920f * 4.0f, FrontLayerZ));
			floorTr->SetPosition(Vector3(0.0f, -1.920f, ObjectZ));
		}


		// land
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(0.0f, -4.350f, 0.0f), Vector3(62.0f, 1.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// right rimit
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(30.50f, 2.950f, 0.0f), Vector3(1.0f, 14.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}
		// left rimit
		{
			GameObject* landObject = objects::InstantiateLandObject(
				this, Vector3(-30.50f, 2.950f, 0.0f), Vector3(1.0f, 14.0f, 1.0f));
			landObject->SetName(L"LandObj");
		}

		// 이 아래로는 플렛폼오브젝트
		// first
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-26.6750f, 1.6750f, 0.0f), Vector3(8.50f, 0.50f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// first right
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-20.0f, -0.770f, 0.0f), Vector3(7.20f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, -0.7850f);
			landObject->SetName(L"PlatformObj");
		}
		// first right bottom
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-16.950f, -3.450f, 0.0f), Vector3(0.70f, 0.50f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// second
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-0.34750f, 0.150f, 0.0f), Vector3(17.30f, 1.0f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// second left
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-10.7550f, -1.480f, 0.0f), Vector3(5.50f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, 0.7850f);
			landObject->SetName(L"PlatformObj");
		}
		// second left bottom
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(-13.4250f, -3.450f, 0.0f), Vector3(1.30f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, 0.0f);
			landObject->SetName(L"PlatformObj");
		}
		// second right
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(10.1250f, -1.480f, 0.0f), Vector3(5.50f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, -0.7850f);

			landObject->SetName(L"PlatformObj");
		}
		// second right bottom
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(12.70f, -3.450f, 0.0f), Vector3(1.30f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, 0.0f);
			landObject->SetName(L"PlatformObj");
		}

		// third
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(25.70f, 1.40f, 0.0f), Vector3(8.0f, 1.0f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
		// third left
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(19.30f, -0.870f, 0.0f), Vector3(7.20f, 0.50f, 1.0f));
			landObject->GetTransform()->SetRotation(0.0f, 0.0f, 0.7850f);
			landObject->SetName(L"PlatformObj");
		}
		// third left bottom
		{
			GameObject* landObject = objects::InstantiatePlatformObject(
				this, Vector3(16.30f, -3.450f, 0.0f), Vector3(0.70f, 0.50f, 1.0f));
			landObject->SetName(L"PlatformObj");
		}
	}
	void Scene_Town::addUIObjects()
	{
	}
	void Scene_Town::addGameObjects()
	{
		GameObject* portalObject = objects::InstantiateGameObject<GameObject>(this, enums::eLayerType::Portal, L"AnimationMaterial");
		portalObject->GetTransform()->SetPosition(math::Vector3(-3.0f, -2.70f, 0.0f));
		TownPortalScript* portalScript = portalObject->AddComponent<TownPortalScript>();
		portalScript->SetExitPosition(math::Vector3(-3.0f, -0.20f, 0.0f));
	}
}
