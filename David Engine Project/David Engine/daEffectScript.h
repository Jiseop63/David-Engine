#pragma once
#include "daScript.h"
#include "daGameObject.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class EffectScript : public Script
	{
	public:
		EffectScript();
		virtual ~EffectScript();

		virtual void Initialize();
		virtual void LateUpdate();

	public:
		Transform* GetTransform() { return mEffectTransform; }
		
		void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3); }
		math::Vector3 GetEffectPosition() { return mEffectTransform->GetPosition(); }
		void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3); }
		math::Vector3 GetEffectRotation() { return mEffectTransform->GetRotation(); }
		void SetEffectScale(math::Vector3 vector3) { mEffectTransform->SetScale(vector3); }
		math::Vector3 GetEffectScale() { return mEffectTransform->GetScale(); }
		void SetReverse(bool value) { mReverse = value; }

	protected:
		Transform*		mEffectTransform;
		MeshRenderer*	mEffectRenderer;
		Animator*		mEffectAnimator;

	private:
		bool mReverse;
	};
}