#include "daMonsterScript.h"

#include "daGameObject.h"
#include "daCreatureLifebarScript.h"
#include "daCombatScript.h"
#include "daMonsterActionUnitScript.h"

namespace da
{
	MonsterScript::MonsterScript()
		: mMonsterSensorCollider(nullptr)
		, mPlayerScript(nullptr)
		, mMonsterCombatScript(nullptr)
		, mMonsterLifeScript(nullptr)
		, mMonsterAttackStat{}
		, mDetectPlayer(0.0f)
	{
	}
	MonsterScript::~MonsterScript()
	{
	}
	void MonsterScript::Initialize()
	{
		CreatureScript::Initialize();
		mMonsterSensorCollider = GetOwner()->AddComponent<Collider2D>();
	}
	void MonsterScript::AddActionUnit(GameObject* unit)
	{
		MonsterActionUnitScript* actionUnit = unit->AddComponent<MonsterActionUnitScript>();
		actionUnit->SetOwnerScript(this);
		mActionUnits.push_back(actionUnit);
	}

	CreatureLifebarScript* MonsterScript::SetCreatureLifeScript(CreatureLifebarScript* creature)
	{
		mMonsterLifeScript = creature;
		mMonsterLifeScript->SetCreatureScript(this);
		mMonsterLifeScript->SetValue(&mCreatureStat.MaxHP, &mCreatureStat.CurHP);
		return creature;
	}
}
