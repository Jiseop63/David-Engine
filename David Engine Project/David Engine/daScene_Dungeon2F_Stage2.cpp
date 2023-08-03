#include "daScene_Dungeon2F_Stage2.h"
// юс╫ц
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
	}
	void Scene_Dungeon2F_Stage2::OnExit()
	{
	}
	void Scene_Dungeon2F_Stage2::addBackgroundObjects()
	{
	}
	void Scene_Dungeon2F_Stage2::addGameObjects()
	{
		// Stage : 352 304
		{
			GameObject* stageObject = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::FrontBackGround, L"2FStage2Material");
			stageObject->GetTransform()->SetScale(math::Vector3(3.520f * 4.0f, 3.040f * 4.0f, 1.0f));
			stageObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, ObjectZ));
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