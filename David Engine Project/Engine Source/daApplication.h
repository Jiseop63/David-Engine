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
		// ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������
		std::unique_ptr<da::graphics::GraphicDevice_Dx11> graphicDevice;

		HWND mHwnd;

		// ������ Ŭ���̾�Ʈ ũ��
		UINT mClientWidth;
		UINT mClientHeight;

		// ������ ������ ũ��
		UINT mFrameWidth;
		UINT mFrameHeight;
	};
}
