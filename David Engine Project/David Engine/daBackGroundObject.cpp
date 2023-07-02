#include "daBackGroundObject.h"

namespace da
{
	BackGroundObject::BackGroundObject()
	{
	}
	BackGroundObject::~BackGroundObject()
	{
	}

	void BackGroundObject::Initialize()
	{		
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
