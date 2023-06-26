#pragma once
#include "daGameObject.h"

namespace da
{
	class OverlayObject : public GameObject
	{
	public:
		OverlayObject();
		virtual ~OverlayObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	};
}