#include "daOverlayObject.h"

namespace da
{
	OverlayObject::OverlayObject()
	{
	}
	OverlayObject::~OverlayObject()
	{
	}
	void OverlayObject::Initialize()
	{
		GameObject::Initialize();
	}
	void OverlayObject::Update()
	{
		GameObject::Update();
	}
	void OverlayObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void OverlayObject::Render()
	{
		GameObject::Render();
	}
}
