#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectEnemyScript : public EffectScript
	{
	public:
		EffectEnemyScript();
		virtual ~EffectEnemyScript();

		virtual void Initialize() override;
	};
}