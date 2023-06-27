#include "daBackGroundObject.h"
#include "daGameManager.h"

namespace da
{
	BackGroundObject::BackGroundObject()
		: mBackGroundScript(nullptr)
	{
	}
	BackGroundObject::~BackGroundObject()
	{
	}

	void BackGroundObject::Initialize()
	{
		mBackGroundScript = AddComponent<BackGroundScript>();
		GameObject::Initialize();
	}
	void BackGroundObject::Update()
	{
	/*	if (true == mIsDisappearObject && true == GameManager::IsOverrayOn())
			GameObject::SetObjectState(GameObject::eObjectState::Disappear);*/

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
