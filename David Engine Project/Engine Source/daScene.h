#pragma once
#include "daEntity.h"

namespace da
{
	class Layer;
	class GameObject;
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		virtual void OnEnter();
		virtual void OnExit();
		
		void AddGameObject(enums::eLayerType layerType, GameObject* target);

	private:
		std::vector<Layer> mLayers;
	};
}