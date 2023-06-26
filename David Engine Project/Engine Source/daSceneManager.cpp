#include "daSceneManager.h"

#include "daTitleScene.h"
#include "daSelectScene.h"
#include "daPlayScene.h"


namespace da
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes = {};
	void SceneManager::Initialize()
	{
		mActiveScene = new PlayScene();
		mActiveScene->Initialize();
		mScenes.insert( std::make_pair(L"PlayScene", mActiveScene) );
		//mScenes.insert(std::make_pair(L"TitleScene", new TitleScene()));
		//mScenes.insert(std::make_pair(L"SelectScene", new SelectScene()));
		//mScenes.insert(std::make_pair(L"PlayScene", new PlayScene()));

		/*for (const auto& scene : mScenes) {
			scene.second->Initialize();
		}

		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(L"TitleScene");
		if (iter != mScenes.end())
		{
			mActiveScene = iter->second;
		}*/
	}
	void SceneManager::Update()
	{
		mActiveScene->Update();
	}
	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}
	void SceneManager::Render()
	{
		mActiveScene->Render();
	}
	void SceneManager::Release()
	{
		for (auto iter : mScenes)
		{
			delete iter.second;
			iter.second = nullptr;
		}

	}
	Scene* SceneManager::LoadScene(std::wstring name)
	{
		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);
		if (iter == mScenes.end())
			return nullptr;
		
		mActiveScene->OnExit();
		mActiveScene = iter->second;
		mActiveScene->OnEnter();
		return mActiveScene;
	}
}
