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

		virtual void Initialize();

		void PlayEffect(const std::wstring name);

		void SetEffectPosition(math::Vector3 vector3) { mEffectTransform->SetPosition(vector3 + (mEffectTransform->Up() * 0.50f)); }
		void SetEffectRotation(math::Vector3 vector3) { mEffectTransform->SetRotation(vector3 + math::Vector3(0.0f, 0.0f, -1.570f)); }

		//void retInactive() { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }

		// 가용 오브젝트를 확ㅇㄴ?하는 함수

	protected:
		Transform*	mEffectTransform;
		Animator*	mEffectAnimator;

	};
}