#pragma once
#include "David Engine.h"
#include "daGraphicDevice Dx11.h"

namespace da
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();

		void SetWindow(HWND hwnd, UINT width, UINT height);

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }

	private:
		bool mbInitialize = false;
		// ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������
		std::unique_ptr<da::graphics::GraphicDevice_Dx11> graphicDevice;

		HWND mHwnd;
		UINT mWidth;
		UINT mHeight;
	};
}
