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

		void SetValue(float max, float cur) { mMaxValue = max, mCurValue = cur; }

	protected:
		float mMaxValue;
		float mCurValue;
	};
}