#include "daGameObject.h"
#include "daRenderer.h"

#include "daTransform.h"

namespace da
{
	GameObject::GameObject()
		: mObjectState(eObjectState::Active)
		, mLayerType(enums::eLayerType::Default)
		, mIsCommonObject(false)
		, mTransform(nullptr)
		, mBodyCollider(nullptr)
		, mFootCollider(nullptr)
		, mComponents{}
		, mScripts{}
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
	void GameObject::Update()
	{
		if (eObjectState::Paused == mObjectState)
			return;
		/*if (eObjectState::Hide == mObjectState)
			return;*/
		if (eObjectState::Inactive == mObjectState)
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
		if (eObjectState::Hide == mObjectState)
			return;
		if (eObjectState::Inactive == mObjectState)
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
		if (eObjectState::Hide == mObjectState)
			return;
		if (eObjectState::Inactive == mObjectState)
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
	void GameObject::SetObjectStates(eObjectState state)
	{
		for (size_t index = 0; index < mChildren.size(); index++)
		{
			mChildren[index]->SetObjectState(state);
		}
		mObjectState = state;
	}
}
