#pragma once
#include "daActionUnitScript.h"

namespace da
{
	class BansheeCombatScript;
	class BansheeProjectileScript : public ActionUnitScript
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