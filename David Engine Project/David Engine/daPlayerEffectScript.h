#pragma once
#include "daEffectScript.h"

namespace da
{
	class PlayerEffectScript : public EffectScript
	{
	public:
		PlayerEffectScript();
		virtual ~PlayerEffectScript();

		virtual void Initialize() override;

	};
}