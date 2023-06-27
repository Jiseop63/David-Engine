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

		void AddGameObject(GameObject* target);

		std::vector<GameObject*> GetGameObjects() { return mGameObjects; }
		// 이건 아마 안쓸듯
		std::vector<GameObject*> GetDisappearObject();
	private:
		std::vector<GameObject*> mGameObjects;
	};
}