#include "daTime.h"
#include "daApplication.h"
#include "daFontWrapper.h"

extern da::Application application;

namespace da
{
	double Time::mDeltaTime = 0.0l;
	double Time::mSecond = 0.0f;
	float Time::mTimeScale = 1.0f;
	LARGE_INTEGER Time::mCpuFrequency = {};
	LARGE_INTEGER Time::mPrevFrequency = {};
	LARGE_INTEGER Time::mCurFrequency = {};

	void Time::Initialize()
	{
		// CPU 고유 진동수 가져오기
		QueryPerformanceFrequency(&mCpuFrequency);

		// 프로그램이 처음 시작할때 진동수
		QueryPerformanceCounter(&mPrevFrequency);
	}

	void Time::Update()
	{
		QueryPerformanceCounter(&mCurFrequency);

		double differnceFrequency = (double)mCurFrequency.QuadPart - mPrevFrequency.QuadPart;

		mDeltaTime = differnceFrequency / mCpuFrequency.QuadPart;

		mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
	}

	void Time::Render()
	{
		mSecond += mDeltaTime;
		wchar_t szFloat[50] = L"BIN";

		if (mSecond > 1.0f)
		{
			HWND hWnd = application.GetHwnd();
						
			float FPS = 1.0f / (float)mDeltaTime;
			swprintf_s(szFloat, 50, L"FPS : %d", (UINT)FPS);
			SetWindowText(hWnd, szFloat);
			mSecond = 0.0f;
		}

		FontWrapper::DrawFont(szFloat, 10.f, 30.f, 20, FONT_RGBA(255, 0, 255, 255));
	}
}
