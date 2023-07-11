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
			if (true == target->IsGlobal())
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
}
