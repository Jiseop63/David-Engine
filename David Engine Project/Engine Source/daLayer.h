#pragma once
#include "daGameObject.h"

namespace da
{
	class Layer
	{
	public:
		Layer();
		~Layer();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();

		void AddGameObject(GameObject* target);

		const std::vector<GameObject*> GetGameObjects() { return mGameObjects; }

	private:
		std::vector<GameObject*> mGameObjects;
	};
}