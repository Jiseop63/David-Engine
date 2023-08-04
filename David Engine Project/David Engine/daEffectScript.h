#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daGameObject.h"
namespace da
{
	class EffectScript : public Script
	{
	public:
		EffectScript();
		virtual ~EffectScript();

		virtual void Initialize();

	public:
		virtual void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3); }
		virtual void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3); }

	protected:
		Transform*	mEffectTransform;
		Animator*	mEffectAnimator;

	};
}