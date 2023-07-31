#pragma once
#include "daScript.h"
#include "daAnimator.h"

namespace da
{
	class EffectScript : public Script
	{
	public:
		EffectScript();
		virtual ~EffectScript();

		virtual void Initialize() override;
		virtual void Update() override;

		void PlayEffect();

		void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3); }
		void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3 + math::Vector3(0.0f, 0.0f, -1.570f)); }

	protected:
		Transform*	mEffectTransform;
		Animator*	mEffectAnimator;
	};
}