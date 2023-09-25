#pragma once
#include "daActionUnitScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	class SkellBossScript;
	class SkellBossProjectileScript : public ActionUnitScript
	{
	public:
		SkellBossProjectileScript();
		virtual ~SkellBossProjectileScript();

		virtual void Initialize() override;
		
	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
	};
}