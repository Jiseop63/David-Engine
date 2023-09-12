#include "daMonsterScript.h"
#include "daGameObject.h"
#include "daCreatureLifebarScript.h"
#include "daEffectEnemyScript.h"
#include "daEnemyWeaponScript.h"

namespace da
{
	MonsterScript::MonsterScript()
		: mMonsterSensorCollider(nullptr)
		, mPlayerScript(nullptr)
		, mCreatureWeaponScript(nullptr)
		, mMonsterLifeScript(nullptr)
		, mMonsterAttackStat{}
		, mMonsterActiveState(eMonsterState::Idle)
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
	void MonsterScript::AddEffectObject(GameObject* effectObject)
	{
		EffectEnemyScript* enemyEffect = effectObject->AddComponent<EffectEnemyScript>();
		mEffects.push_back(enemyEffect);
	}

#pragma region Init other script
	void MonsterScript::SetEnemyWeaponScript(EnemyWeaponScript* creature)
	{
		mCreatureWeaponScript = creature;
	}
	CreatureLifebarScript* MonsterScript::SetCreatureLifeScript(CreatureLifebarScript* creature)
	{
		mMonsterLifeScript = creature;
		mMonsterLifeScript->SetCreatureScript(this);
		mMonsterLifeScript->SetValue(&mCreatureStat.MaxHP, &mCreatureStat.CurHP);
		return creature;
	}
#pragma endregion
}
