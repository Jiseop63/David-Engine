#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectWeaponScript : public EffectScript
	{
	public:
		EffectWeaponScript();
		virtual ~EffectWeaponScript();

		virtual void Initialize() override;

		virtual void PlayEffect(const std::wstring name) override;
		virtual void retInactive() override { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }

	public:
		virtual void SetEffectPosition(math::Vector3 vector3) override { mEffectTransform->SetPosition(vector3 + (mEffectTransform->Up() * 0.20f)); }
		virtual void SetEffectRotation(math::Vector3 vector3) override { mEffectTransform->SetRotation(vector3 + math::Vector3(0.0f, 0.0f, -1.570f)); }


	};
}