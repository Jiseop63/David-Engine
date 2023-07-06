#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"
#include "daScene.h"

namespace da
{
	class Application
	{
	public:
		Application();
		~Application();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();

	public:
		HWND GetHwnd() { return mHwnd; }
		HDC  GetHDC() { return GetDC(mHwnd); }
		UINT GetClientWidth() { return mClientWidth; }
		UINT GetClientHeight() { return mClientHeight; }
		UINT GetFrameWidth() { return mFrameWidth; }
		UINT GetFrameHeight() { return mFrameHeight; }

	public:
		void Run();
		void SetWindow(HWND hwnd, UINT width, UINT height);
		void CalculateWindowSize(UINT width, UINT height);

	private:
		bool mbInitialize = false;
		// 오로지 한개의 객체만 만들수 있는 스마트 포인터
		std::unique_ptr<da::graphics::GraphicDevice_Dx11> graphicDevice;

		HWND mHwnd;

		// 윈도우 클라이언트 크기
		UINT mClientWidth;
		UINT mClientHeight;

		// 윈도우 프레임 크기
		UINT mFrameWidth;
		UINT mFrameHeight;
	};
}
