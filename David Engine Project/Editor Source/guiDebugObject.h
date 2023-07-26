#pragma once
#include "..\\Engine SOURCE\\daGameObject.h"

namespace gui
{
	class DebugObject : public da::GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		void BindColor(da::math::Vector4 color);
	};
}