#include "daENVObject.h"

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
		GameObject::Update();
	}
	void ENVObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void ENVObject::Render()
	{
		GameObject::Render();
	}
}
