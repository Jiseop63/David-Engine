#include "daPortalIconScript.h"
#include "daGameObject.h"
#include "daResources.h"

namespace da
{
	PortalIconScript::PortalIconScript()
		: mIconTransform(nullptr)
		, mIconRenderer(nullptr)
	{
	}
	PortalIconScript::~PortalIconScript()
	{
	}
	void PortalIconScript::Initialize()
	{
		mIconTransform = GetOwner()->GetTransform();
		mIconRenderer = GetOwner()->GetComponent<MeshRenderer>();
				
		mIconTransform->SetScale(math::Vector3(0.650f, 0.70f, 1.0f));
	}
	void PortalIconScript::SetPosition(math::Vector3 position)
	{
		mIconTransform->SetPosition(position + math::Vector3(0.0f, 0.3f, 0.0f));
	}
}
