#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	class FairyScript : public Script
	{
	public:
		FairyScript();
		virtual ~FairyScript();

		virtual void Initialize();

		virtual void OnCollisionEnter(Collider2D* other);

		void SetPosition(math::Vector3 position) { mFairyTransform->SetPosition(position); }
		void SetPosition(float x, float y) { mFairyTransform->SetPosition(math::Vector3(x, y, 0.0f)); }

		void DungeonFinished();

	protected:
		Transform* mFairyTransform;
		Collider2D* mFairyCollider;
		Animator* mFairyAnimator;
	};
}