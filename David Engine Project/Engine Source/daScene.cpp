#include "daScene.h"

namespace da
{
	using namespace enums;
	Scene::Scene()
	{
		mLayers.resize((int)eLayerType::End);
	}
	Scene::~Scene()
	{
	}
	void Scene::Initialize()
	{
		for (Layer& targetLayer : mLayers)
		{
			targetLayer.Initialize();
		}
	}
	void Scene::Update()
	{
		for (Layer& targetLayer : mLayers)
		{
			targetLayer.Update();
		}
	}
	void Scene::LateUpdate()
	{
		for (Layer& targetLayer : mLayers)
		{
			targetLayer.LateUpdate();
		}
	}
	void Scene::Render()
	{
		for (Layer& targetLayer : mLayers)
		{
			targetLayer.Render();
		}
	}
	void Scene::AddGameObject(eLayerType layerType, GameObject* target)
	{
		mLayers[(UINT)layerType].AddGameObject(target);
	}
}
