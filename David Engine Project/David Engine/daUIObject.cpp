#include "daUIObject.h"

namespace da
{
	UIObject::UIObject()
	{
	}
	UIObject::~UIObject()
	{
	}
	void UIObject::Initialize()
	{
		GameObject::Initialize();
	}
	void UIObject::Update()
	{
		GameObject::Update();
	}
	void UIObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void UIObject::Render()
	{
		GameObject::Render();
	}
}
