#include "daPortalWallScript.h"
#include "daGameObject.h"
namespace da
{
	PortalWallScript::PortalWallScript()
		: mWallTransform(nullptr)
		, mWallCollider(nullptr)
	{
	}

	PortalWallScript::~PortalWallScript()
	{
	}

	void PortalWallScript::Initialize()
	{
		mWallTransform = GetOwner()->GetTransform();
		mWallCollider = GetOwner()->AddComponent<Collider2D>();

		//mWallTransform->SetScale()
		mWallCollider->SetSize(math::Vector2(2.40f, 0.50f));
		mWallCollider->SetDetectionType(Collider2D::eDetectionType::Env);
		mWallCollider->ApplyComponentUsing(false);
	}
}