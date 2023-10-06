#pragma once
#include "daLifeBarScript.h"

namespace da
{
	class CreatureScript;
	class CreatureLifebarScript : public LifeBarScript
	{
	public:
		CreatureLifebarScript();
		virtual ~CreatureLifebarScript();

		virtual void Update() override;


	public:
		void SetCreatureScript(CreatureScript* creature) { mCreatureScript = creature; }
	private:
		CreatureScript* mCreatureScript;
	};
}