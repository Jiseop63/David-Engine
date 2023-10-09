#include "daMonsterScript.h"

#include "daGameObject.h"
#include "daCreatureLifebarScript.h"
#include "daCombatScript.h"
#include "daMonsterActionUnitScript.h"
#include "daAudioSource.h"
#include "daResources.h"

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
	void MonsterScript::OnDamaged(float damage)
	{
		mCreatureStat.CurHP -= damage;
		if (0 >= mCreatureStat.CurHP)
			mCreatureStat.CurHP = 0;
		mCreatureAudio->Play(Resources::Find<AudioClip>(L"Hit_Monster"), 40.0f);
	}
	void MonsterScript::MonsterDeadEffects()
	{
		mMonsterCombatScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
		ActionUnitScript* actionUnit = CreatureScript::callActionUnit();

		structs::sActionUnitTypes effectUnitTypes = {};
		effectUnitTypes.Usage = enums::eUnitUsageType::JustAnimation;
		effectUnitTypes.LifeCycle = enums::eUnitLifeType::AnimationEnd;
		effectUnitTypes.Action = enums::eUnitActionType::Stay;
		actionUnit->SetUnitTypes(effectUnitTypes);

		structs::sActionUnitStat effectUnitInfo = {};
		effectUnitInfo.Animation.Action = L"Dying";
		actionUnit->SetUnitInfo(effectUnitInfo);
		actionUnit->SetUnitScale(math::Vector3(1.20f, 1.20f, 1.0f));

		actionUnit->SetUnitReverse(isLeft());

		actionUnit->SetUnitOffset(math::Vector3(0.0f, -0.20f, 0.0f));
		actionUnit->OnActive();
		mIsDead = true;

		mCreatureAudio->Play(Resources::Find<AudioClip>(L"MonsterDie"), 50.0f);
	}
}
