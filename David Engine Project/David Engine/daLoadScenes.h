#pragma once
#include "daSceneManager.h"
#include "daScene_Title.h"

#ifdef _DEBUG
#pragma comment(lib, "../x64/Debug/David Engine.lib")
#else
#pragma comment(lib, "../x64/Release/David Engine.lib")
#endif // _DEBUG


namespace da
{
	void InitializeScenes()
	{
		SceneManager::CreateScene<Scene_Title>(L"Scene_Title");
	}
}