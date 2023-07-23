#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	class CreatureScript : public Script
	{
	public:
		CreatureScript();
		virtual ~CreatureScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;

	protected:
		Rigidbody* mRigidbody;
		Animator* mAnimator;


	};
}