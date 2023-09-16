#pragma once
#include "daCreatureScript.h"

namespace da
{
	enum class eMonsterState
	{
		Idle,
		Chase,
		Attack,
		Dead,
	};
	class PlayerScript;
	class EffectEnemyScript;
	class CombatScript;
	class CreatureLifebarScript;
	class MonsterScript : public CreatureScript
	{

	public:
		MonsterScript();
		virtual ~MonsterScript();

		virtual void Initialize();


	public:
		virtual void AddEffectObject(GameObject* effectObject) override;		
		void SetEnemyWeaponScript(class CombatScript* creature);
		CreatureLifebarScript* SetCreatureLifeScript(CreatureLifebarScript* creature);

		// ���� ���� ������ �����ϴ� ���
		void SetDetectRange(float value)
		{
			mMonsterAttackStat.DetectRange = value;
			mMonsterSensorCollider->SetSize(math::Vector2(value * 2.0f, 0.90f));
		}
		void SetAttackRange(float value) { mMonsterAttackStat.AttackRange = value; }

		// player�� ���� ȣ���
		void MonsterFindsPlayer(bool value) { mDetectPlayer = value; }

#pragma region Components
	protected:
		Collider2D*				mMonsterSensorCollider;
		PlayerScript*			mPlayerScript;
		CombatScript*			mMonsterCombatScript;
		CreatureLifebarScript*	mMonsterLifeScript;
#pragma endregion

	protected:
		structs::sMonsterAttackStat mMonsterAttackStat;
		eMonsterState				mMonsterActiveState;
		bool						mDetectPlayer;

	};
}