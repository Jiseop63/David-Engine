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
		ENVObject::Initialize();
	}
	void BackGroundObject::Update()
	{
		ENVObject::Update();
	}
	void BackGroundObject::LateUpdate()
	{
		ENVObject::LateUpdate();
	}
	void BackGroundObject::Render()
	{
		ENVObject::Render();
	}
}
