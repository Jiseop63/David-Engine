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
		// �̸����� �� ã��
		std::map<std::wstring, Scene*>::iterator iter = mScenes.find(name);
		if (iter == mScenes.end())
			return nullptr;

		// CommonObject �ű��
		MoveCommonObjects();

		// ���� �� Exit ȣ��
		if (nullptr != mActiveScene)
			mActiveScene->OnExit();

		// ���� �� ����
		mActiveScene = iter->second;

		// ���� �� Enter ȣ��
		mActiveScene->OnEnter();


		return mActiveScene;
	}
	void SceneManager::MoveCommonObjects()
	{
		// Dont Destroy Object ��������
		std::vector<GameObject*> commonObjects = mActiveScene->GetCommonObjects();

		// ������Ʈ �ű��
		for (GameObject* target : commonObjects)
		{
			enums::eLayerType type = target->GetLayerType();
			mActiveScene->AddGameObject(type, target);
		}
	}
}
