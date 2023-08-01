#include "daSceneManager.h"

namespace da
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes = {};
	void SceneManager::Initialize()
	{		
	}
	void SceneManager::Update()
	{
		mActiveScene->Update();
	}
	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
	}	
	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
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
		// 이름으로 씬 찾기
		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);
		if (iter == mScenes.end())
			return nullptr;

		// CommonObject 옮기기
		MoveCommonObjects();

		// 기존 씬 Exit 호출
		if (nullptr != mActiveScene)
			mActiveScene->OnExit();

		// 현재 씬 변경
		mActiveScene = iter->second;

		// 현재 씬 Enter 호출
		mActiveScene->OnEnter();


		return mActiveScene;
	}
	void SceneManager::MoveCommonObjects()
	{
		// Dont Destroy Object 가져오기
		std::vector<GameObject*> commonObjects = mActiveScene->GetCommonObjects();

		// 오브젝트 옮기기
		for (GameObject* target : commonObjects)
		{
			enums::eLayerType type = target->GetLayerType();
			mActiveScene->AddGameObject(type, target);
		}
	}
}
