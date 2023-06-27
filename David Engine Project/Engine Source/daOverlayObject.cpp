#include "daOverlayObject.h"
#include "daGameManager.h"

namespace da
{
	OverlayObject::OverlayObject()
	{
		SetObjectState(eObjectState::Paused);
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
		if ( false == GameManager::IsOverrayOn() )
			return;
		GameObject::Update();
	}
	void OverlayObject::LateUpdate()
	{
		if (false == GameManager::IsOverrayOn())
			return;
		GameObject::LateUpdate();
	}
	void OverlayObject::Render()
	{
		if (false == GameManager::IsOverrayOn())
			return;
		GameObject::Render();
	}
}
