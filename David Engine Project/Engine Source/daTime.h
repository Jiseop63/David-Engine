#pragma once
#include "David Engine.h"

namespace da
{
	class Time
	{
	public:
		static void Initialize();
		static void Update();
		static void Render();


		static void ApplyTimeScale(float scale) { mTimeScale = scale; }
		static void ClearTimeScale() { mTimeScale = 1.0f; }

		__forceinline static double DeltaTime() { return mDeltaTime; }
		__forceinline static double DeltaScaleTime() { return mDeltaTime * mTimeScale; }

	private:
		static double mDeltaTime;
		static double mSecond;
		static float mTimeScale;
		static LARGE_INTEGER mCpuFrequency;
		static LARGE_INTEGER mPrevFrequency;
		static LARGE_INTEGER mCurFrequency;
	};
}
