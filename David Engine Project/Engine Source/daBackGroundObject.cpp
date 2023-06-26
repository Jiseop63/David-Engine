#include "daBackGroundObject.h"
#include "daTransform.h"
#include "daBackGroundObject.h"
#include "daGameManager.h"
namespace da
{
	BackGroundObject::BackGroundObject()
		: mTransform(nullptr)
		, mBackGroundScript(nullptr)
		, mIsDisapperObject(false)
	{
	}
	BackGroundObject::~BackGroundObject()
	{
	}

	void BackGroundObject::Initialize()
	{
		SetObjectType(eObjectType::Background);
		mTransform = GetComponent<Transform>();
		mBackGroundScript = AddComponent<BackGroundScript>();
		GameObject::Initialize();
	}
	void BackGroundObject::Update()
	{
		if (true == mIsDisapperObject && true == GameManager::IsOverrayOn())
			GameObject::SetObjectState(GameObject::eObjectState::Paused);

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
