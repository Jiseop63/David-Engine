#pragma once
#include "daEffectScript.h"

namespace da
{
	class BansheeCombatScript;
	class BansheeAttackEffectScript : public EffectScript
	{
	public:
		BansheeAttackEffectScript();
		virtual ~BansheeAttackEffectScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


		virtual void OnActive() override;



	private:
		void projectileHitEffect();
		void retInactive();


	public:
		void SetReqWeapon(BansheeCombatScript* weapon)
		{
			mReqWeapon = weapon;
		}

	private:
		bool					mChanged;
		BansheeCombatScript* mReqWeapon;
	};
}