#pragma once
#include "daFlyingCreatureScript.h"

namespace da
{
	class PlayerScript;
	class EffectEnemyScript;
	class CombatScript;
	class CreatureLifebarScript;
	class FlyingMonsterScript : public FlyingCreatureScript
	{

	public:
		FlyingMonsterScript();
		virtual ~FlyingMonsterScript();

		virtual void Initialize();


	public:
		virtual void AddEffectObject(GameObject* effectObject) override;
		void SetEnemyWeaponScript(CombatScript* creature) { mMonsterCombatScript = creature; }
		CreatureLifebarScript* SetCreatureLifeScript(CreatureLifebarScript* creature);

		// 세부 몬스터 스텟을 설정하는 기능
		void SetDetectRange(float value)
		{
			mMonsterSensorCollider->SetSize(math::Vector2(value * 2.0f, 0.90f));
		}
		void SetAttackRange(float value) { mMonsterAttackStat.AttackRange = value; }

		// player에 의해 호출됨
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
		bool						mDetectPlayer;

	};
}