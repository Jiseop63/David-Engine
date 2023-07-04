#include "daScene_Dungeon.h"


// юс╫ц
#include "daInput.h"
#include "daSceneManager.h"

// resource
#include "daResources.h"
#include "daTexture.h"

// objects & components
#include "daObjects.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"
#include "daTransform.h"

#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"

#include "daTimeConstants.h"

#include "daUIObject.h"
#include "daButtonScript.h"


namespace da
{
	Scene_Dungeon::Scene_Dungeon()
	{
	}
	Scene_Dungeon::~Scene_Dungeon()
	{
	}
	void Scene_Dungeon::Initialize()
	{
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		addBackgroundObjects();
	}
	void Scene_Dungeon::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon");
		}
		Scene::Update();
	}
	void Scene_Dungeon::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void Scene_Dungeon::Render()
	{
		Scene::Render();
	}
	
	void Scene_Dungeon::OnEnter()
	{
	}
	void Scene_Dungeon::OnExit()
	{
	}
	void Scene_Dungeon::addBackgroundObjects()
	{
		// TempDungeonMaterial
		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::Layer, L"TempDungeonMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
		}
	}
}
