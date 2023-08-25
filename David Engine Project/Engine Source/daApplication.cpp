#include "daApplication.h"
#include "daInput.h"
#include "daTime.h"
#include "daRenderer.h"
#include "daSceneManager.h"
#include "daCollisionManager.h"

namespace da
{
	Application::Application()
		: graphicDevice(nullptr)
		, mHwnd(NULL)
		, mClientWidth(-1)
		, mClientHeight(-1)
	{

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
		Destroy();
	}

	void Application::Initialize()
	{
		Time::Initialize();
		Input::Initialize();

		renderer::Initialize();
		SceneManager::Initialize();
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();
		CollisionManager::Update();
		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		SceneManager::LateUpdate();
	}

	void Application::Render()
	{
		Time::Render();
		//Input::Render();

		graphicDevice->ClearTarget();
		graphicDevice->UpdateViewPort();
		
		renderer::Render();
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}

	void Application::Present()
	{
		graphicDevice->Present();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		// 최초 실행시 unique 객체를 초기화 해줌
		if (graphicDevice == nullptr)
		{
			mHwnd = hwnd;
			mClientWidth = width;
			mClientHeight = height;
			CalculateWindowSize(hwnd, width, height);
			
			graphicDevice = std::make_unique<da::graphics::GraphicDevice_Dx11>();
			da::graphics::GetDevice() = graphicDevice.get();
		}

		RECT winRect = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, UINT(winRect.right - winRect.left), UINT(winRect.bottom - winRect.top), 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

	void Application::CalculateWindowSize(HWND hwnd, UINT width, UINT height)
	{
		// 윈도우 크기 구하기
		RECT winRect = {};
		GetClientRect(hwnd, &winRect);
		mFrameWidth = UINT(winRect.right - winRect.left);
		mFrameHeight = UINT(winRect.bottom - winRect.top);
		
	}
}
