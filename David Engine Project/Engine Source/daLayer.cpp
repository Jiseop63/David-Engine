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
			targetObject->Update();
		}
	}
	void Layer::LateUpdate()
	{
		for (GameObject* targetObject : mGameObjects)
		{
			targetObject->LateUpdate();
		}
	}
	void Layer::Render()
	{
		for (GameObject* targetObject : mGameObjects)
		{
			targetObject->Render();
		}
	}
	void Layer::AddGameObject(GameObject* target)
	{
		mGameObjects.push_back(target);
	}
	std::vector<GameObject*> Layer::GetDisappearObject()
	{
		std::vector<GameObject*> retDisapperObject;
		for (GameObject* targetObject : mGameObjects)
		{
			if (GameObject::eObjectState::Disappear == targetObject->GetObjectState())
				retDisapperObject.push_back(targetObject);
		}
		return retDisapperObject;
	}
}
