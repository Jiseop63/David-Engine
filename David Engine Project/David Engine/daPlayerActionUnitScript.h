#pragma once
#include "daActionUnitScript.h"

namespace da
{
	class PlayerActionUnitScript : public ActionUnitScript
	{
	public:
		PlayerActionUnitScript();
		virtual ~PlayerActionUnitScript();

		virtual void Initialize() override;


		virtual void OnCollisionEnter(Collider2D* other) override;

	};
}