#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectWeaponScript : public EffectScript
	{
	public:
		EffectWeaponScript();
		virtual ~EffectWeaponScript();

		virtual void Initialize() override;
		void retInactive();
	};
}