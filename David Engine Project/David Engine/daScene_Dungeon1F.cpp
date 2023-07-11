#include "daScene_Dungeon1F.h"

#include "daRenderer.h"

// 임시
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
	Scene_Dungeon1F::Scene_Dungeon1F()
	{
	}
	Scene_Dungeon1F::~Scene_Dungeon1F()
	{
	}
	void Scene_Dungeon1F::Initialize()
	{
		mMainCamera = objects::InstantiateMainCamera(this);
		mUICamera = objects::InstantiateUICamera(this);

		objects::InstantiateGridObject(this, mMainCamera);
		addBackgroundObjects();
		addUIObjects();
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
		renderer::mainCamera = mMainCamera->GetCameraComponent();
		renderer::uiCamera = mUICamera->GetCameraComponent();
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
	void Scene_Dungeon1F::addUIObjects()
	{

		// Player Life
		{
			// HUD 객체 생성
			GameObject* lifeHUD = objects::InstantiateObject
				<GameObject>(this, enums::eLayerType::None);
			Transform* lifeHUDTransform = lifeHUD->GetTransform();
			//  HUD 위치 조절 (좌상단)
			lifeHUDTransform->SetPosition(Vector3(-MaxPositionX, MaxPositionY, HUDZ));
			float padding = 0.20f;
			// Panel 객체 생성
			GameObject* lifePanel = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifePanelMaterial");
			Transform* lifePanelTransform = lifePanel->GetTransform();
			lifePanelTransform->SetParent(lifeHUDTransform);
			// Panel 크기 조절
			float panelXScale = 2.960f;	// 74 * 4
			float panelYScale = 0.640f;	// 16 * 4
			lifePanelTransform->SetScale(Vector3(panelXScale, panelYScale, 1.0f));
			// Panel 위치 조절
			Vector3 lifePanelPosition = Vector3((panelXScale / 2.0f) + padding, -(panelYScale / 2.0f) - padding, 0.f);
			lifePanelTransform->SetPosition(lifePanelPosition);
			// Bar 객체 생성
			GameObject* lifeBar = objects::InstantiateGameObject
				<GameObject>(this, enums::eLayerType::UI, L"PlayerLifeBarMaterial");
			Transform* lifeBarTransform = lifeBar->GetTransform();
			lifeBarTransform->SetParent(lifeHUDTransform);
			// Bar 크기 조절
			float barXScale = 1.960f;	// 49 * 4
			float barYScale = 0.40f;	// 10 * 4
			lifeBarTransform->SetScale(Vector3(barXScale, barYScale, 1.0f));
			// Bar 위치 조절하기
			// 22 * 4
			// hud 위치에서 panel size의 절반만큼 당긴다음에... bar offset만큼 더 당기면될듯?
			Vector3 lifeBarPosition = lifePanelPosition + Vector3(0.380f, 0.0f, -0.0001f);
			lifeBarTransform->SetPosition(lifeBarPosition);
		}
	}
}
