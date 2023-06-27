#include "daENVObject.h"
#include "daGameManager.h"

namespace da
{
	ENVObject::ENVObject()
		: mIsDisappearObject(false)
	{
	}
	ENVObject::~ENVObject()
	{
	}
	void ENVObject::Initialize()
	{
		GameObject::Initialize();
	}
	void ENVObject::Update()
	{
		if (true == mIsDisappearObject && GameManager::IsOverrayOn())
			return;
		GameObject::Update();
	}
	void ENVObject::LateUpdate()
	{
		if (true == mIsDisappearObject && GameManager::IsOverrayOn())
			return;
		GameObject::LateUpdate();
	}
	void ENVObject::Render()
	{
		if (true == mIsDisappearObject && GameManager::IsOverrayOn())
			return;
		GameObject::Render();
	}
}
