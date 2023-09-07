#include "daMonsterScript.h"
#include "daGameObject.h"
#include "daCreatureLifebarScript.h"
#include "daEffectEnemyScript.h"

namespace da
{
	MonsterScript::MonsterScript()
	{
	}
	MonsterScript::~MonsterScript()
	{
	}
	void MonsterScript::Initialize()
	{
		CreatureScript::Initialize();
	}
	void MonsterScript::AddEffectObject(GameObject* effectObject)
	{
		EffectEnemyScript* enemyEffect = effectObject->AddComponent<EffectEnemyScript>();
		mEffects.push_back(enemyEffect);
	}
	EnemyWeaponScript* MonsterScript::SetEnemyWeaponScript(EnemyWeaponScript* creature)
	{
		mCreatureWeaponScript = creature;
		return creature;
	}
	CreatureLifebarScript* MonsterScript::SetCreatureLifeScript(CreatureLifebarScript* creature)
	{
		mMonsterLifeScript = creature;
		mMonsterLifeScript->SetCreatureScript(this);
		mMonsterLifeScript->SetValue(mCreatureStat->MaxHP, mCreatureStat->CurHP);
		return creature;
	}
}
