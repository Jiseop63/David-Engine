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
	void Layer::Initialize()
	{
		for (GameObject* targetObject : mGameObjects)
		{
			targetObject->Initialize();
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
		// dead ������Ʈ ��Ƶα�
		std::set<GameObject*> deleteObjects = {};
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj->GetObjectState()
				== GameObject::eObjectState::Destroy)
				deleteObjects.insert(gameObj);
		}

		// daed ������Ʈ ���ܽ�Ű�� layer����
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
		//�޸� ����
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
}
