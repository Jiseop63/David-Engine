#pragma once
#include "daMonsterActionUnitScript.h"

namespace da
{
	class BansheeCombatScript;
	class BansheeProjectileScript : public MonsterActionUnitScript
	{
	public:
		BansheeProjectileScript();
		virtual ~BansheeProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		void projectileHitEffect();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void ClearUnit() override;

	private:
		bool mChnaged;
	};
}