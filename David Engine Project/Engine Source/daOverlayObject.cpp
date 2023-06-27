#include "daOverlayObject.h"
#include "daTransform.h"

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
		mTransform = GetComponent<Transform>();
		GameObject::Initialize();
	}
	void OverlayObject::Update()
	{
		GameObject::Update();
		// Overlay가 On인 경우 업데이트하기
	}
	void OverlayObject::LateUpdate()
	{
		// Overlay가 On인 경우 업데이트하기
		GameObject::LateUpdate();
	}
	void OverlayObject::Render()
	{
		// Overlay가 On인 경우 업데이트하기
		GameObject::Render();
	}
}
