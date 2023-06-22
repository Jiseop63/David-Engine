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
		for (Component* component : mComponents)
		{
			if (nullptr == component)
				continue;

			delete component;
			component = nullptr;
		}

		for (Script* script : mScripts)
		{
			if (script == nullptr)
				continue;

			delete script;
			script = nullptr;
		}
	}
	void GameObject::Initialize()
	{
	}
	void GameObject::Update()
	{
		for (Component* component : mComponents)
		{
			component->Update();
		}
		for (Script* script : mScripts)
		{
			script->Update();
		}
	}
	void GameObject::LateUpdate()
	{
		for (Component* component : mComponents)
		{
			component->LateUpdate();
		}
		for (Script* script : mScripts)
		{
			script->LateUpdate();
		}
	}
	void GameObject::Render()
	{
		for (Component* component : mComponents)
		{
			component->Render();
		}
		for (Script* script : mScripts)
		{
			script->Render();
		}
	}
}
