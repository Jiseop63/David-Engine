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

	protected:
		Transform* mFairyTransform;
		Collider2D* mFairyCollider;
		Animator* mFairyAnimator;
	};
}