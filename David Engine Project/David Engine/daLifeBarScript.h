#pragma once
#include "daScript.h"

namespace da
{
	class LifeBarScript : public Script
	{
	public:
		LifeBarScript();
		virtual ~LifeBarScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	};
}