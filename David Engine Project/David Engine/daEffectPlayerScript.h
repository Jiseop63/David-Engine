#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectPlayerScript : public EffectScript
	{
	public:
		EffectPlayerScript();
		virtual ~EffectPlayerScript();

		virtual void Initialize() override;
		
	};
}