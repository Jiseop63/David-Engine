#include "daBackGroundObject.h"
#include "daTransform.h"
#include "daBackGroundObject.h"

namespace da
{
	BackGroundObject::BackGroundObject()
		: mTransform(nullptr)
		, mBackGroundScript(nullptr)
	{
	}
	BackGroundObject::~BackGroundObject()
	{
	}

	void BackGroundObject::Initialize()
	{
		mTransform = GetComponent<Transform>();
		mBackGroundScript = AddComponent<BackGroundScript>();
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
