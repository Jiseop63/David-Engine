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
		Transform* GetTransform() { return mEffectTransform; }
		
		void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3); }
		math::Vector3 GetEffectPosition() { return mEffectTransform->GetPosition(); }
		void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3); }
		math::Vector3 GetEffectRotation() { return mEffectTransform->GetRotation(); }

	protected:
		Transform*	mEffectTransform;
		Animator*	mEffectAnimator;

	};
}