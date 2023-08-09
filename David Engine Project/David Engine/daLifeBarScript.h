#pragma once
#include "daScript.h"

namespace da
{
	class LifeBarScript : public Script
	{
	public:
		LifeBarScript();
		virtual ~LifeBarScript();

		void BindConstantBuffer();
	};
}