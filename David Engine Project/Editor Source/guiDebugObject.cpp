#include "guiDebugObject.h"
#include "daTransform.h"

namespace gui
{
	DebugObject::DebugObject()
	{
		AddComponent<da::Transform>();
	}

	DebugObject::~DebugObject()
	{
	}

	void DebugObject::Update()
	{
		GameObject::Update();
	}

	void DebugObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void DebugObject::Render()
	{
		GameObject::Render();
	}
}
