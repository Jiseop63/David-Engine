#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"
#include "daTransform.h"

namespace da
{
	CameraObject::CameraObject()
		: mCameraComponent(nullptr)
	{
		mCameraComponent = AddComponent<Camera>();
		mTransform->SetPosition(math::Vector3(0.0f, 0.0f, -6.750f));
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