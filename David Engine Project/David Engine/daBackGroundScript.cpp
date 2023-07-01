#include "daBackGroundScript.h"

#include "daTime.h"

#include "daGameManager.h"

#include "daGameObject.h"
#include "daTransform.h"

namespace da
{
	using namespace math;

	BackGroundScript::BackGroundScript()
		: mTransform(nullptr)
	{
	}
	BackGroundScript::~BackGroundScript()
	{
	}

	void BackGroundScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}
	void BackGroundScript::Update()
	{		
	}
	void BackGroundScript::LateUpdate()
	{
	}
	void BackGroundScript::Render()
	{
	}
}