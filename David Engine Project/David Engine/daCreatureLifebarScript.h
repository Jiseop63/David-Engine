#pragma once
#include "daScript.h"

namespace da
{
	class CreatureScript;
	class CreatureLifebarScript : public Script
	{
	public:
		CreatureLifebarScript();
		virtual ~CreatureLifebarScript();

		virtual void Update() override;

		void BindConstantBuffer();

	public:
		void SetCreatureScript(CreatureScript* creature) { mCreatureScript = creature; }

	private:
		CreatureScript* mCreatureScript;
	};
}