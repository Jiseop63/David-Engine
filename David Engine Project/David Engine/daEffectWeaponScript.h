#pragma once
#include "daEffectScript.h"

namespace da
{
	class WeaponScript;
	class EffectWeaponScript : public EffectScript
	{
	public:
		EffectWeaponScript();
		virtual ~EffectWeaponScript();

		virtual void Initialize() override;

		void PlayEffect(enums::eWeaponName weaponType);
		void PlayEffect(const std::wstring name);
		void MeleeEffect(const std::wstring name);
		void retInactive();

		// 외부 세팅 함수
	public:
		void SetReqWeapon(WeaponScript* weapon) 
		{ 
			mReqWeapon = weapon;
			int a = 0;
		}

	private:
		WeaponScript* mReqWeapon;
	};
}