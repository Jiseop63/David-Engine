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

	private:
		std::vector<GameObject*> mGameObjects;
	};
}