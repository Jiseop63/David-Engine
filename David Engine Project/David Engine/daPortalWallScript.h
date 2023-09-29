#pragma once
#include "daScript.h"

namespace da
{
	class PortalWallScript : public Script
	{
	public:
		PortalWallScript();
		virtual ~PortalWallScript();

		virtual void Initialize() override;

	public:
		void SetPosition(math::Vector3 position) { mWallTransform->SetPosition(position); }
		void SetColliderSize(math::Vector2 size) { mWallCollider->SetSize(size); }

	public:
		void ApplyCollision(bool value = true)
		{
			if (value)
				mWallCollider->ApplyComponentUsing();
			else
				mWallCollider->ApplyComponentUsing(false);
		}

	private:
		Transform*	mWallTransform;
		Collider2D* mWallCollider;
	};
}