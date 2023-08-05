#pragma once
#include "daEffectScript.h"
#include "daWeaponScript.h"

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
		void retInactive();

		// �ܺ� ���� �Լ�
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