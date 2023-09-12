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

		void SetValue(const float* max, const float* cur) { mMaxValue = max, mCurValue = cur; }

	protected:
		const float* mMaxValue;
		const float* mCurValue;
	};
}