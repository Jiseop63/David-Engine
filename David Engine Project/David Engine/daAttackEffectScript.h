#pragma once
#include "daEffectScript.h"

namespace da
{
	class CombatScript;
	class AttackEffectScript : public EffectScript
	{
	public:
		AttackEffectScript();
		virtual ~AttackEffectScript();

		virtual void Initialize() override;

		void PlayEffect(enums::eWeaponName weaponType);
		void PlayEffect(const std::wstring name);
		void MeleeEffect(const std::wstring name);
		void retInactive();

		// 외부 세팅 함수
	public:
		void SetReqWeapon(CombatScript* weapon) 
		{ 
			mReqWeapon = weapon;
			int a = 0;
		}

	private:
		CombatScript* mReqWeapon;
	};
}