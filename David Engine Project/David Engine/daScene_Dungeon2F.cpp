#include "daScene_Dungeon2F.h"

#include "daRenderer.h"
// юс╫ц
#include "daInput.h"
#include "daSceneManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"

// UI, Object, Components and Camera
#include "daObjects.h"
#include "daObjectsFastIncludeHeader.h"


namespace da
{
	Scene_Dungeon2F::Scene_Dungeon2F()
	{
	}
	Scene_Dungeon2F::~Scene_Dungeon2F()
	{
	}
	void Scene_Dungeon2F::Initialize()
	{
		mMainCamera = objects::InstantiateMainCamera(this);
		mUICamera = objects::InstantiateUICamera(this);

		objects::InstantiateGridObject(this, mMainCamera);
		addBackgroundObjects();
	}
	void Scene_Dungeon2F::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Title");
		}
		Scene::Update();
	}
	void Scene_Dungeon2F::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon2F::Render()
	{
		Scene::Render();
	}

	void Scene_Dungeon2F::OnEnter()
	{
		renderer::mainCamera = mMainCamera->GetCameraComponent();
		renderer::uiCamera = mUICamera->GetCameraComponent();
	}
	void Scene_Dungeon2F::OnExit()
	{
	}
	void Scene_Dungeon2F::addBackgroundObjects()
	{
		// TempDungeonMaterial
		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::Layer, L"2FMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
		}
	}
}
