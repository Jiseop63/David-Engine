#pragma once
#include "..\\Engine SOURCE\\daGameObject.h"

namespace gui
{
	class DebugObject : public da::GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();

		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	};
}