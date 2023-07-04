#include "daScene_Town.h"
#include "daInput.h"
#include "daSceneManager.h"
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
	}
	void Scene_Town::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"Scene_Title");
		}
	}
	void Scene_Town::LateUpdate()
	{
	}
	void Scene_Town::Render()
	{
	}
	
	void Scene_Town::OnEnter()
	{
	}
	void Scene_Town::OnExit()
	{
	}

	void Scene_Town::addBackgroundObject()
	{
	}
}
