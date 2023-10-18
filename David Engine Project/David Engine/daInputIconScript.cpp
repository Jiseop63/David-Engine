#include "daInputIconScript.h"
#include "daGameObject.h"
#include "daResources.h"

namespace da
{
	InputIconScript::InputIconScript()
		: mIconTransform(nullptr)
		, mIconRenderer(nullptr)
	{
	}
	InputIconScript::~InputIconScript()
	{
	}
	void InputIconScript::Initialize()
	{
		mIconTransform = GetOwner()->GetTransform();
		mIconRenderer = GetOwner()->GetComponent<MeshRenderer>();
				
		mIconTransform->SetScale(math::Vector3(0.650f, 0.70f, 1.0f));
	}
	void InputIconScript::SetPosition(math::Vector3 position)
	{
		mIconTransform->SetPosition(position + math::Vector3(0.0f, 0.3f, 0.0f));
	}
}
