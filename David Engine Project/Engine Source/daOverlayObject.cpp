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
		// Overlay�� On�� ��� ������Ʈ�ϱ�
	}
	void OverlayObject::LateUpdate()
	{
		// Overlay�� On�� ��� ������Ʈ�ϱ�
		GameObject::LateUpdate();
	}
	void OverlayObject::Render()
	{
		// Overlay�� On�� ��� ������Ʈ�ϱ�
		GameObject::Render();
	}
}
