#pragma once
#include "daSceneManager.h"
#include "daScene_Title.h"
#include "daScene_Town.h"
#include "daScene_Dungeon.h"
#include "daScene_Dungeon2F.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\David Engine.lib")
//#pragma comment(lib, "../x64/Debug/David Engine.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\David Engine.lib")
//#pragma comment(lib, "../x64/Release/David Engine.lib")
#endif // _DEBUG

namespace da
{
	void InitializeScenes()
	{
		SceneManager::CreateScene<Scene_Title>(L"Scene_Title");
		SceneManager::CreateScene<Scene_Town>(L"Scene_Town");
		SceneManager::CreateScene<Scene_Dungeon>(L"Scene_Dungeon");
		SceneManager::CreateScene<Scene_Dungeon2F>(L"Scene_Dungeon2F");
	}
}
