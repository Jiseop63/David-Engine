#pragma once
#include "daSceneManager.h"
#include "daScene_Title.h"
#include "daScene_Town.h"

#include "daScene_dungeon1_1.h"
#include "daScene_dungeon1_2.h"
#include "daScene_dungeon1_3.h"
#include "daScene_dungeon1_4.h"
#include "daScene_dungeon1_5.h"
#include "daScene_dungeon1_final.h"

#include "daScene_dungeon2_1.h"
#include "daScene_dungeon2_boss.h"



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
		SceneManager::CreateScene<Scene_dungeon1_1>(L"Scene_dungeon1_1");
		SceneManager::CreateScene<Scene_dungeon1_2>(L"Scene_dungeon1_2");
		SceneManager::CreateScene<Scene_dungeon1_3>(L"Scene_dungeon1_3");
		SceneManager::CreateScene<Scene_dungeon1_4>(L"Scene_dungeon1_4");
		SceneManager::CreateScene<Scene_dungeon1_5>(L"Scene_dungeon1_5");
		SceneManager::CreateScene<Scene_dungeon1_final>(L"Scene_dungeon1_final");

		SceneManager::CreateScene<Scene_dungeon2_1>(L"Scene_dungeon2_1");
		SceneManager::CreateScene<Scene_dungeon2_boss>(L"Scene_dungeon2_boss");

		da::SceneManager::LoadScene(L"Scene_Title");
	}
}
