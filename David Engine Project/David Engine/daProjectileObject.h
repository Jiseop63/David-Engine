#pragma once
#include "daGameObject.h"

namespace da
{
	class ProjectileObject : public GameObject
	{
	public:
		ProjectileObject();
		virtual ~ProjectileObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	};
}
