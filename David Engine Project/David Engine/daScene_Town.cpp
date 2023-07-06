#include "daScene_Town.h"

// 임시
#include "daInput.h"
#include "daSceneManager.h"
#include "daApplication.h"
#include "daRenderer.h"

// resource
#include "daResources.h"
#include "daTexture.h"


// UI, Object, Components and Camera
#include "daObjecsFastIncludeHeader.h"

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
		mMainCamera = objects::InstantiateMainCamera(this);
		mUICamera = objects::InstantiateUICamera(this);
		addBackgroundObject();
		addUIObjects();
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Dungeon1F");
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
		// 카메라 세팅
		renderer::mainCamera = mMainCamera->GetCameraComponent();
		renderer::uiCamera = mUICamera->GetCameraComponent();
		
		// 플레이어 세팅
	}
	void Scene_Town::OnExit()
	{
	}

	void Scene_Town::addBackgroundObject()
	{
		float width = (float)application.GetClientWidth();
		width /= 100.0f;
		float height = (float)application.GetClientHeight();
		height /= 100.0f;

		// sky BG : stay
		{
			GameObject* backGround = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::BackGround, L"SkyMaterial");
			backGround->GetTransform()->SetScale(math::Vector3(width, height, 1.0f));
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
		// player life panel, dash panel
		{
			GameObject* lifePanel = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");

			Transform* lifePanelTransform = lifePanel->GetTransform();


			float lifeXScale = 2.960f;
			float lifeYScale = 0.640f;



			lifePanelTransform->SetScale(Vector3(lifeXScale, lifeYScale, 1.0f));
			float lifeWidthScaleHalf = lifeXScale / 2.0f;
			float lifeHeightScaleHalf = lifeYScale / 2.0f;
			float lifePanelPadding = 0.20f;
			Vector3 lifePanelPosition = Vector3(-MaxPositionX + lifeWidthScaleHalf + lifePanelPadding, MaxPositionY - lifeHeightScaleHalf - lifePanelPadding, -0.0050f);
			lifePanelTransform->SetPosition(lifePanelPosition);


			GameObject* dashPanel = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"DashPanelMaterial");

			Transform* dashPanelTransform = dashPanel->GetTransform();


			float dashXScale = 0.880f;
			float dashYScale = 0.640f;

			dashPanelTransform->SetScale(Vector3(dashXScale, dashYScale, 1.0f));
			float dashWidthScaleHalf = dashXScale / 2.0f;
			float dashHeightScaleHalf = dashYScale / 2.0f;
			float dashPanePadding = 0.050f;
			Vector3 dashPanelPosition = Vector3(-MaxPositionX + dashWidthScaleHalf + lifePanelPadding, MaxPositionY - (lifeHeightScaleHalf * 2) - dashHeightScaleHalf - lifePanelPadding, -0.0050f);
			dashPanelTransform->SetPosition(dashPanelPosition);
		}
		// player weapon panel
		{
			GameObject* weaponBase = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"WeaponBaseMaterial");
			Transform* weaponPanelTransform = weaponBase->GetTransform();
			float weaponPanelScaleX = 1.70f;
			float weaponPanelScaleY = 1.20f;
			weaponPanelTransform ->SetScale(math::Vector3(weaponPanelScaleX, weaponPanelScaleY, 1.0f));
			float weaponWidthScaleHalf = weaponPanelScaleX / 2.0f;
			float weaponHeightScaleHalf = weaponPanelScaleY / 2.0f;
			float dashPanePadding = 0.150f;

			Vector3 weaponPanelPosition = Vector3(MaxPositionX - weaponWidthScaleHalf - dashPanePadding, -MaxPositionY + weaponHeightScaleHalf + dashPanePadding, -0.0050f);
			weaponPanelTransform ->SetPosition(weaponPanelPosition);
		}
		// mouse
		{
			GameObject* cursorObject = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::UI, L"ShootingCursorMaterial");
			cursorObject->GetTransform()->SetScale(math::Vector3(0.210f * 4.0f, 0.210f * 4.0f, 1.0f));
			cursorObject->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, CursorZ));
			cursorObject->AddComponent<CursorScript>();
		}
	}
}
