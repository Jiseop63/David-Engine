#include "daScene_Dungeon1F.h"


// юс╫ц
#include "daInput.h"
#include "daSceneManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"

// UI, Object, Components and Camera
#include "daObjecsFastIncludeHeader.h"


namespace da
{
	Scene_Dungeon1F::Scene_Dungeon1F()
	{
	}
	Scene_Dungeon1F::~Scene_Dungeon1F()
	{
	}
	void Scene_Dungeon1F::Initialize()
	{
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		addBackgroundObjects();
	}
	void Scene_Dungeon1F::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon2F");
		}
		Scene::Update();
	}
	void Scene_Dungeon1F::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon1F::Render()
	{
		Scene::Render();
	}
	
	void Scene_Dungeon1F::OnEnter()
	{
	}
	void Scene_Dungeon1F::OnExit()
	{
	}
	void Scene_Dungeon1F::addBackgroundObjects()
	{
		// TempDungeonMaterial
		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"TempDungeonMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
		}
	}
}
