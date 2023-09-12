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

		void SetValue(const int* max, const int* cur)
		{ 
			mMaxValue = max;
			mCurValue = cur;
		}

	private:
		const int* mMaxValue;
		const int* mCurValue;
	};
}