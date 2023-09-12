#pragma once
#include "daScript.h"

namespace da
{
	class DashCountScript : public Script
	{
	public:
		DashCountScript();
		virtual ~DashCountScript();

		virtual void LateUpdate();
		void BindConstantBuffer();

		void SetValue(const float& max, const float& cur) { mMaxValue = max; mCurValue = cur; }

	private:
		float mMaxValue;
		float mCurValue;
	};
}