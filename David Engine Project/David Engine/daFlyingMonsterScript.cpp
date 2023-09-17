#include "daFlyingMonsterScript.h"
#include "daGameObject.h"
#include "daCreatureLifebarScript.h"
#include "daEffectEnemyScript.h"
#include "daCombatScript.h"

namespace da
{
	FlyingMonsterScript::FlyingMonsterScript()
		: mMonsterSensorCollider(nullptr)
		, mPlayerScript(nullptr)
		, mMonsterCombatScript(nullptr)
		, mMonsterLifeScript(nullptr)
		, mDetectPlayer(false)
	{
	}
	FlyingMonsterScript::~FlyingMonsterScript()
	{
	}
	void FlyingMonsterScript::Initialize()
	{
		FlyingCreatureScript::Initialize();
		mMonsterSensorCollider = GetOwner()->AddComponent<Collider2D>();
		mMonsterSensorCollider->SetColliderType(enums::eColliderShape::Circle);
	}
	void FlyingMonsterScript::AddEffectObject(GameObject* effectObject)
	{
		EffectEnemyScript* enemyEffect = effectObject->AddComponent<EffectEnemyScript>();
		mEffects.push_back(enemyEffect);
	}
	CreatureLifebarScript* FlyingMonsterScript::SetCreatureLifeScript(CreatureLifebarScript* creature)
	{
		mMonsterLifeScript = creature;
		mMonsterLifeScript->SetFlyingCreatureScript(this);
		mMonsterLifeScript->SetValue(&mCreatureStat.MaxHP, &mCreatureStat.CurHP);
		return creature;
	}
}
