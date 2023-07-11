#pragma once
#include "daEntity.h"
#include "daLayer.h"


#define MaxPositionX 6.830f 
#define MaxPositionY 3.840f

#define BackgroundZ	0.0050f
#define BackLayerZ	0.0040f
#define FrontLayerZ 0.0030f

#define Objects		0.0f;

#define HUDZ		-0.0010f
#define PanelZ		-0.0030f
#define CursorZ		-0.0050f

namespace da
{
	class GameObject;
	class CameraObject;
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
		virtual void OnEnter() {}
		virtual void OnExit() {}
		
		void AddGameObject(enums::eLayerType layerType, GameObject* target);
		
		template <typename T>
		std::vector<T*> FindObjectOfType()
		{
			std::vector<T*> findObjects = {};
			for (Layer* layer : mLayers)
			{
				auto objectLayer = layer->GetGameObjects();
				for (GameObject* object : objectLayer)
				{
					T* target = dynamic_cast<T*>(object);
					if (nullptr != target)
						findObjects.push_back(target);
				}
			}
			return findObjects;
		}

		Layer& GetLayer(enums::eLayerType targetLayer) { return mLayers[(UINT)targetLayer]; }

	protected:
		CameraObject*	mMainCamera;
		CameraObject*	mUICamera;

	private:
		std::vector<Layer> mLayers;
	};
}