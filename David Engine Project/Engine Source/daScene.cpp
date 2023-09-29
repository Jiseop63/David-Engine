#include "daScene.h"
#include "daLayer.h"
#include "daGameObject.h"

namespace da
{
	using namespace enums;
	Scene::Scene()
		: mPortals()
	{
		mLayers.resize((int)eLayerType::End);
	}
	Scene::~Scene()
	{
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
	void Scene::Destroy()
	{
		for (Layer& targetLayer : mLayers)
		{
			targetLayer.Destroy();
		}
	}
	void Scene::AddGameObject(eLayerType layerType, GameObject* target)
	{
		target->SetLayerType(layerType);
		mLayers[(UINT)layerType].AddGameObject(target);
	}
	std::vector<GameObject*> Scene::GetCommonObjects()
	{
		std::vector<GameObject*> retObjects;

		for (Layer& layer : mLayers)
		{
			std::vector<GameObject*> commonObjects
				= layer.GetCommonObjects();

			retObjects.insert(retObjects.end()
				, commonObjects.begin()
				, commonObjects.end());
		}

		return retObjects;
	}
}
