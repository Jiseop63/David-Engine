#include "daLayer.h"

namespace da
{
	Layer::Layer()
	{
	}
	Layer::~Layer()
	{
		for (GameObject* target : mGameObjects)
		{
			if (nullptr == target)
				continue;

			delete target;
			target = nullptr;
		}
	}
	void Layer::Update()
	{
		for (GameObject* targetObject : mGameObjects)
		{
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Paused)
				continue;

			targetObject->Update();
		}
	}
	void Layer::LateUpdate()
	{
		for (GameObject* targetObject : mGameObjects)
		{
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Hide)
				continue;

			targetObject->LateUpdate();
		}
	}
	void Layer::Render()
	{
		for (GameObject* targetObject : mGameObjects)
		{			
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Inactive)
				continue;
			if (targetObject->GetObjectState()
				== GameObject::eObjectState::Hide)
				continue;

			targetObject->Render();
		}
	}
	void Layer::Destroy()
	{
		// dead 오브젝트 모아두기
		std::set<GameObject*> deleteObjects = {};
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj->GetObjectState()
				== GameObject::eObjectState::Destroy)
				deleteObjects.insert(gameObj);
		}

		// daed 오브젝트 제외시키기 layer에서
		typedef std::vector<GameObject*>::iterator GameObjectIter;
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end(); )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteObjects.find(*(iter));

			if (deleteIter != deleteObjects.end())
			{
				iter = mGameObjects.erase(iter);
				continue;
			}

			iter++;
		}
		//메모리 해제
		for (GameObject* gameObj : deleteObjects)
		{
			delete gameObj;
			gameObj = nullptr;
		}
	}
	void Layer::AddGameObject(GameObject* target)
	{
		mGameObjects.push_back(target);
	}

	// 호출되는 시점에서, 레이어에 있는 common객체들을 옮긴다
	std::vector<GameObject*> Layer::GetCommonObjects()
	{
		std::vector<GameObject*> retObjects;

		for ( std::vector<GameObject*>::iterator iter = mGameObjects.begin()
			; iter != mGameObjects.end(); )
		{
			if ((*iter)->IsCommonObject())
			{
				retObjects.push_back((*iter));
				iter = mGameObjects.erase(iter);
			}
			else
				iter++;
		}
		return retObjects;
	}
	void Layer::ObjectsActive()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->SetObjectState(GameObject::eObjectState::Active);
		}
	}
	void Layer::ObjectsPaused()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->SetObjectState(GameObject::eObjectState::Paused);
		}
	}
}
