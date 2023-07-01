#include "daBackGroundObject.h"

namespace da
{
	BackGroundObject::BackGroundObject()
		: mBackGroundScript(nullptr)
	{
	}
	BackGroundObject::~BackGroundObject()
	{
	}

	void BackGroundObject::Initialize()
	{
		mBackGroundScript = AddComponent<BackGroundScript>();
		GameObject::Initialize();
	}
	void BackGroundObject::Update()
	{
		GameObject::Update();
	}
	void BackGroundObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void BackGroundObject::Render()
	{
		GameObject::Render();
	}
}
