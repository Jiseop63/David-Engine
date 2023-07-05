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
		addUIObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon");
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

		// 레이어 역할을 하는 배경들은 스케일 값을 건드는게 아니라 카메라로부터 영향받는 수치를 낮추는 식으로 해야할듯?
		// town BG : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownBGMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -3.0f, -0.001f));
		}

		// town Layer : follow
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::Layer, L"TownLayerMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(13.66f, 7.68f, 1.0f));
			backGround->GetTransform()->SetPosition(math::Vector3(0.0f, -6.0f, -0.002f));
		}
	}
	void Scene_Town::addUIObjects()
	{
		// player life bar
		{
			GameObject* LifeBack = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBackMaterial");
			LifeBack->GetTransform()->SetScale(math::Vector3(2.220f, 0.480f, 1.0f));
			LifeBack->GetTransform()->SetPosition(math::Vector3(-4.750f, 2.90f, -0.005f));
			LifeBack->SetName(L"1");
			GameObject* LifeBase = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBaseMaterial");
			LifeBase->GetTransform()->SetScale(math::Vector3(2.220f, 0.480f, 1.0f));
			LifeBase->GetTransform()->SetPosition(math::Vector3(-4.750f, 2.90f, -0.006f));
			LifeBase->SetName(L"2");

		}
		// player dash panel
		{
			GameObject* dashCount = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"DashCountBaseMaterial");
			dashCount->GetTransform()->SetScale(math::Vector3(1.890f, 0.240f, 1.0f));
			dashCount->GetTransform()->SetPosition(math::Vector3(-4.80f, 2.50f, -0.005f));
			dashCount->SetName(L"3");

		}
		// player weapon panel
		{
			GameObject* weaponBase = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			weaponBase->GetTransform()->SetScale(math::Vector3(1.70f, 1.20f, 1.0f));
			weaponBase->GetTransform()->SetPosition(math::Vector3(4.5f, -2.5f, -0.005f));
		}

	}
}
