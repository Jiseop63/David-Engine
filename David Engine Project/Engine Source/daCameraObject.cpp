#include "daCameraObject.h"
#include "daCamera.h"
#include "daCameraScript.h"
#include "daTransform.h"

namespace da
{
	CameraObject::CameraObject()
		: mTransform(nullptr)
		, mCameraComponent(nullptr)
		, mCameraScript(nullptr)
	{
	}
	CameraObject::~CameraObject()
	{
	}
	void CameraObject::Initialize()
	{
		mTransform = GetComponent<Transform>();
		mCameraComponent = AddComponent<Camera>();
		mCameraScript = AddComponent<CameraScript>();
		mTransform->SetPosition(math::Vector3(0.0f, 0.0f, -10.0f));
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