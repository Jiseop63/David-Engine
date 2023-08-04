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

		void PlayEffect(enums::eWeaponType weaponType);
		void MeleeEffect(const std::wstring name);
		void retInactive() { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }
	};
}