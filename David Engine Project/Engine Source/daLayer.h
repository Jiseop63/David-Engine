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
		// �̰� �Ƹ� �Ⱦ���
		std::vector<GameObject*> GetDisappearObject();
	private:
		std::vector<GameObject*> mGameObjects;
	};
}