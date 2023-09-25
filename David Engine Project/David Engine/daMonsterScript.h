#pragma once
#include "daCreatureScript.h"

namespace da
{	
	class PlayerScript;
	class CombatScript;
	class CreatureLifebarScript;
	class MonsterScript : public CreatureScript
	{

	public:
		MonsterScript();
		virtual ~MonsterScript();

		virtual void Initialize();

	public:
		virtual void AddActionUnit(GameObject* unit) override;
		CreatureLifebarScript* SetCreatureLifeScript(CreatureLifebarScript* creature);

		// ���� ���� ������ �����ϴ� ���
		void SetDetectRange(float value) { mMonsterSensorCollider->SetSize(math::Vector2(value * 2.0f, 0.90f)); }
		void SetAttackRange(float value) { mMonsterAttackStat.AttackRange = value; }

		// player�� ���� ȣ���
		void MonsterFindsPlayer(bool value) { mDetectPlayer = value; }

#pragma region Components
	protected:
		Collider2D*				mMonsterSensorCollider;
		CombatScript*			mMonsterCombatScript;
		CreatureLifebarScript*	mMonsterLifeScript;
		PlayerScript*			mPlayerScript;
#pragma endregion

	protected:
		structs::sMonsterAttackStat mMonsterAttackStat;
		
		bool						mDetectPlayer;

	};
}