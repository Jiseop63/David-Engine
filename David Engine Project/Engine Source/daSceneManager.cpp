#include "daSceneManager.h"


namespace da
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes = {};

	CameraScript* SceneManager::mMainCameraScript = nullptr;
	SubCameraScript* SceneManager::mSubCameraScript = nullptr;
	PlayerScript* SceneManager::mPlayerScript = nullptr;
	InventoryScript* SceneManager::mInventoryScript = nullptr;
	LifeBarScript* SceneManager::mLifebarScript = nullptr;
	DashCountScript* SceneManager::mDashCountScript = nullptr;
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


		std::vector<GameObject*> commonObjects;
		// prev Scene�� �ִٸ� ������ ����
		if (mActiveScene)
		{
			// Dont Destroy Object ��������
			commonObjects = mActiveScene->GetCommonObjects();
			// Exit ȣ��
			mActiveScene->OnExit();
		}

		// �� �����ϱ�
		mActiveScene = iter->second;

		// ������Ʈ �ű��
		for (GameObject* target : commonObjects)
		{
			enums::eLayerType type = target->GetLayerType();
			mActiveScene->AddGameObject(type, target);
		}		

		int a = 0;
		// ���� �� Enter ȣ��
		mActiveScene->OnEnter();


		return mActiveScene;
	}
	void SceneManager::MoveCommonObjects()
	{
		


		
	}
}
