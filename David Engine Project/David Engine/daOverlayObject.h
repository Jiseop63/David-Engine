#pragma once
#include "daUIObject.h"

namespace da
{
	class OverlayObject : public UIObject
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