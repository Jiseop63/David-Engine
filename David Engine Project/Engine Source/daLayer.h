#pragma once
#include "daGameObject.h"

namespace da
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		void Update();
		void LateUpdate();
		void Render();
		void Destroy();

		void AddGameObject(GameObject* target);
		const std::vector<GameObject*> GetGameObjects() { return mGameObjects; }
		std::vector<GameObject*> GetCommonObjects();

		void ObjectsActive();
		void ObjectsPaused();

	private:
		std::vector<GameObject*> mGameObjects;
	};
}