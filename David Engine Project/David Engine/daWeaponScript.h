#pragma once
#include "daScript.h"

namespace da
{
	class WeaponScript : public Script
	{
	public:
		WeaponScript();
		virtual ~WeaponScript();

		virtual void Update() override;
	};
}