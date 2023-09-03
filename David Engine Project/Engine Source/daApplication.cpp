#include "daApplication.h"
#include "daInput.h"
#include "daTime.h"
#include "daRenderer.h"
#include "daSceneManager.h"
#include "daCollisionManager.h"
#include "daFmod.h"
#include "daFontWrapper.h"

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
		Fmod::Initialize();
		FontWrapper::Initialize();

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
		graphicDevice->ClearTarget();
		graphicDevice->UpdateViewPort();

		Time::Render();
		//Input::Render();
		//FontWrapper::DrawFont(L"TEXT", 10.f, 30.f, 20, FONT_RGBA(255, 0, 255, 255));

		
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

			graphicDevice = std::make_unique<da::graphics::GraphicDevice_Dx11>();
			da::graphics::GetDevice() = graphicDevice.get();
		}

		CalculateWindowSize(width, height);

		SetWindowPos(mHwnd, nullptr, 0, 0, mFrameWidth, mFrameHeight, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

	void Application::CalculateWindowSize(UINT width, UINT height)
	{
		// 윈도우 크기 구하기
		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		mFrameWidth = UINT(rt.right - rt.left);
		mFrameHeight = UINT(rt.bottom - rt.top);
	}
}
