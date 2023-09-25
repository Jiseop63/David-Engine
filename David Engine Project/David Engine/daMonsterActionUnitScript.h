#pragma once
#include "daActionUnitScript.h"

namespace da
{
	class MonsterActionUnitScript : public ActionUnitScript
	{
	public:
		MonsterActionUnitScript();
		virtual ~MonsterActionUnitScript();

		virtual void Initialize() override;

	};
}