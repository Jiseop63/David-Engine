#pragma once
#include "daEntity.h"
#include "daLayer.h"

namespace da
{
	class GameObject;
	class CameraObject;
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void Initialize() {}
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();
	public:
		virtual void OnEnter() {}
		virtual void OnExit() {}
		
		void AddGameObject(enums::eLayerType layerType, GameObject* target);
		
		std::vector<GameObject*> GetCommonObjects();
		template <typename T>
		std::vector<T*> GetTypeOfObjects()
		{
			std::vector<T*> retObjects = {};
			for (Layer* layer : mLayers)
			{
				auto objectLayer = layer->GetGameObjects();
				for (GameObject* object : objectLayer)
				{
					T* target = dynamic_cast<T*>(object);
					if (nullptr != target)
						retObjects.push_back(target);
				}
			}
			return retObjects;
		}
		Layer& GetLayer(enums::eLayerType targetLayer) { return mLayers[(UINT)targetLayer]; }

	private:
		std::vector<Layer> mLayers;
	};
}