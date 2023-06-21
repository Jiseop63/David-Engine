#include "daGameObject.h"
#include "daRenderer.h"
#include "daTransform.h"

namespace da
{
	GameObject::GameObject()
		: mState(eObjectState::Active)
	{
		AddComponent<Transform>();
	}
	GameObject::~GameObject()
	{
		for (Component* target : mComponents)
		{
			if (nullptr == target)
				continue;

			delete target;
			target = nullptr;
		}
	}
	void GameObject::Initialize()
	{
	}
	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			comp->Update();
		}
	}
	void GameObject::LateUpdate()
	{
		for (Component* comp : mComponents)
		{
			comp->LateUpdate();
		}
	}
	void GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			comp->Render();
		}
	}
}
