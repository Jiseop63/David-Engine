#include "daCameraObject.h"
#include "daCamera.h"
#include "daTransform.h"

namespace da
{
	CameraObject::CameraObject()
		: mCameraComponent(nullptr)
	{
		
	}
	CameraObject::~CameraObject()
	{
	}
	void CameraObject::Initialize()
	{		
		GameObject::Initialize();
	}
	void CameraObject::Update()
	{
		GameObject::Update();
	}
	void CameraObject::LateUpdate()
	{
		GameObject::LateUpdate();
	}
	void CameraObject::Render()
	{
		GameObject::Render();
	}
}