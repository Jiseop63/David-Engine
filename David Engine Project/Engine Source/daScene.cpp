#include "daScene.h"

namespace da
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}
	void Scene::Initialize()
	{
		for (Layer* gameObj : mLayers)
		{
			gameObj->Initialize();
		}
	}
	void Scene::Update()
	{
		for (Layer* gameObj : mLayers)
		{
			gameObj->Update();
		}
	}
	void Scene::LateUpdate()
	{
		for (Layer* gameObj : mLayers)
		{
			gameObj->LateUpdate();
		}
	}
	void Scene::Render()
	{
		for (Layer* gameObj : mLayers)
		{
			gameObj->Render();
		}
	}
}
