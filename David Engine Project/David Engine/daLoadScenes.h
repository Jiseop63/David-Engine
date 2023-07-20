#pragma once
#include "daSceneManager.h"
#include "daScene_Title.h"
#include "daScene_Town.h"
#include "daScene_Dungeon1F_Stage1.h"
#include "daScene_Dungeon2F_Stage1.h"
#include "daScene_Dungeon2F_Stage2.h"

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
		SceneManager::CreateScene<Scene_Dungeon1F_Stage1>(L"Scene_Dungeon1F_Stage1");
		SceneManager::CreateScene<Scene_Dungeon2F_Stage1>(L"Scene_Dungeon2F_Stage1");
		SceneManager::CreateScene<Scene_Dungeon2F_Stage2>(L"Scene_Dungeon2F_Stage2");

		da::SceneManager::LoadScene(L"Scene_Title");
	}
}
