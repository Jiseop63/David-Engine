#include "daGameObject.h"
#include "daRenderer.h"

#include "daTransform.h"

namespace da
{
	GameObject::GameObject()
		: mObjectState(eObjectState::Active)
		, mTransform(nullptr)
	{
		mTransform = AddComponent<Transform>();
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
		for (Component* component : mComponents)
		{
			component->Initialize();
		}
		for (Script* script : mScripts)
		{
			script->Initialize();
		}
	}
	void GameObject::Update()
	{
		if (eObjectState::Paused == mObjectState)
			return;
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
		if (eObjectState::Paused == mObjectState)
			return;
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
		if (eObjectState::Paused == mObjectState)
			return;
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
