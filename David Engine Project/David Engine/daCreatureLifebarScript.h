#pragma once
#include "daLifeBarScript.h"

namespace da
{
	class CreatureScript;
	class FlyingCreatureScript;
	class CreatureLifebarScript : public LifeBarScript
	{
	public:
		CreatureLifebarScript();
		virtual ~CreatureLifebarScript();

		virtual void Update() override;


	public:
		void SetCreatureScript(CreatureScript* creature) { mCreatureScript = creature; }
		void SetFlyingCreatureScript(FlyingCreatureScript* creature) { mFlyingCreatureScript = creature; }
	private:
		CreatureScript* mCreatureScript;
		FlyingCreatureScript* mFlyingCreatureScript;
	};
}