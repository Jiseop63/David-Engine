#include "daScene_Town.h"

// 임시
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
	Scene_Town::Scene_Town()
	{
	}
	Scene_Town::~Scene_Town()
	{
	}
	void Scene_Town::Initialize()
	{
		CameraObject* mainCameraObj = objects::InstantiateMainCamera(this);
		CameraObject* uiCameraObj = objects::InstantiateUICamera(this);
		addBackgroundObject();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Title");
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
		// 카메라 세팅, 플레이어 세팅
	}
	void Scene_Town::OnExit()
	{
	}

	void Scene_Town::addBackgroundObject()
	{
		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
		}

		// town BG : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.001f));
		}

		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, -0.002f));
		}
	}
}
