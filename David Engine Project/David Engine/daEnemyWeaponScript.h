#pragma once
#include "daScript.h"

namespace da
{
	class EnemyWeaponScript : public Script
	{
	public:
		EnemyWeaponScript();
		virtual ~EnemyWeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;

	};
}