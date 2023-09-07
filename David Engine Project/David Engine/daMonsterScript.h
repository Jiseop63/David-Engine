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

		// 공격기능과 채력바를 외부에서 추가함
		EnemyWeaponScript* SetEnemyWeaponScript(EnemyWeaponScript* creature);
		CreatureLifebarScript* SetCreatureLifeScript(CreatureLifebarScript* creature);

		// 세부 몬스터 스텟을 설정하는 기능
		void SetDetectRange(float value)
		{
			mMonsterAttackStat.DetectRange = value;
			mMonsterSensorCollider->SetSize(math::Vector2(value * 2.0f, 0.90f));
		}
		void SetAttackRange(float value) { mMonsterAttackStat.AttackRange = value; }

		// player에 의해 호출됨
		void MonsterFindsPlayer(bool value) { mDetectPlayer = value; }

	protected:
		Collider2D*			mMonsterSensorCollider;

		PlayerScript*		mPlayerScript;

		EnemyWeaponScript*	mCreatureWeaponScript;			//	일단 보류
		EffectEnemyScript*	mEnemyEffectScript;				//	일단 보류
		CreatureLifebarScript* mMonsterLifeScript;




	protected:
		structs::sMonsterAttackStat mMonsterAttackStat;

	protected:
		eMonsterState			mMonsterActiveState;

		bool					mDetectPlayer;

	};
}