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
	};
}