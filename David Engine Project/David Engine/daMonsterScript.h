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
	class EnemyWeaponScript;
	class EffectEnemyScript;
	class CreatureLifebarScript;
	class MonsterScript : public CreatureScript
	{

	public:
		MonsterScript();
		virtual ~MonsterScript();

		virtual void Initialize();


	public:
		virtual void AddEffectObject(GameObject* effectObject) override;

		// ���ݱ�ɰ� ä�¹ٸ� �ܺο��� �߰���
		EnemyWeaponScript* SetEnemyWeaponScript(EnemyWeaponScript* creature);
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

	protected:
		Collider2D*			mMonsterSensorCollider;

		PlayerScript*		mPlayerScript;

		EnemyWeaponScript*	mCreatureWeaponScript;			//	�ϴ� ����
		EffectEnemyScript*	mEnemyEffectScript;				//	�ϴ� ����
		CreatureLifebarScript* mMonsterLifeScript;




	protected:
		structs::sMonsterAttackStat mMonsterAttackStat;

	protected:
		eMonsterState			mMonsterActiveState;

		bool					mDetectPlayer;

	};
}