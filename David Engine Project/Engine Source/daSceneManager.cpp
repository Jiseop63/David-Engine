#include "daSceneManager.h"
#include "daLayer.h"

namespace da
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes = {};

	GameObject* SceneManager::mLightObject = nullptr;
	CameraScript* SceneManager::mMainCameraScript = nullptr;
	SubCameraScript* SceneManager::mSubCameraScript = nullptr;

	PlayerScript* SceneManager::mPlayerScript = nullptr;
	GameObject* SceneManager::mInventoryObject = nullptr;
	InventoryScript* SceneManager::mInventoryScript = nullptr;
	
	GameObject* SceneManager::mHUDObject = nullptr;
	LifeBarScript* SceneManager::mLifebarScript = nullptr;
	DashCountScript* SceneManager::mDashCountScript = nullptr;
	
	CursorScript* SceneManager::mCursourScrip = nullptr;
	
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


		std::vector<GameObject*> commonObjects;
		// prev Scene이 있다면 진행할 절차
		if (mActiveScene)
		{
			// Dont Destroy Object 가져오기
			commonObjects = mActiveScene->GetCommonObjects();
			
			for (GameObject* target : commonObjects)
			{
				if (enums::eLayerType::PlayableProjectile == target->GetLayerType()
					&& GameObject::eObjectState::Inactive != target->GetObjectState())
				{
					target->SetObjectState(GameObject::eObjectState::Inactive);
				}
				if (enums::eLayerType::MonsterProjectile == target->GetLayerType()
					&& GameObject::eObjectState::Inactive != target->GetObjectState())
				{
					target->SetObjectState(GameObject::eObjectState::Inactive);
				}
				if (enums::eLayerType::BossProjectile == target->GetLayerType()
					&& GameObject::eObjectState::Inactive != target->GetObjectState())
				{
					target->SetObjectState(GameObject::eObjectState::Inactive);
				}
			}
			// Exit 호출
			mActiveScene->OnExit();
		}

		// 씬 변경하기
		mActiveScene = iter->second;

		// 오브젝트 옮기기
		for (GameObject* target : commonObjects)
		{
			enums::eLayerType type = target->GetLayerType();
			mActiveScene->AddGameObject(type, target);
		}
		
		// 현재 씬 Enter 호출
		mActiveScene->OnEnter();
		
		return mActiveScene;
	}
	void SceneManager::ClearProjectileObjects()
	{
	}
}
