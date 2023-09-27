#pragma once
#include "daMonsterScript.h"

namespace da
{
	enum class eBansheeState
	{
		Idle,
		Attack,
		Dead,
	};

	class BansheeScript : public MonsterScript
	{
	public:
		BansheeScript();
		virtual ~BansheeScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		virtual void AddActionUnit(GameObject* unit) override;
		CombatScript* AddCombatObject(GameObject* object);


#pragma region Spawn
		// �ʱ� ��ġ�� ���� ���ϱ�
		// ���� ����Ʈ? �ִϸ��̼�? �����ϱ�

		void retIdle();
#pragma endregion

#pragma region Common Func
	private:
		void calcTargetDir();
		void lifeCheck();
#pragma endregion

#pragma region FSM
	private:
		void ChangeState(eBansheeState state);
		void monsterFSM();
		void BansheeHandleIdle();
		void BansheeHandleAttack();
		void BansheeHandleDead();
#pragma endregion

#pragma region Chase Func
	private:
		void findingPlayer();		// �߰� ���ѷ���
#pragma endregion

#pragma region Attack Func
	private:
		void prepareForAttack();	// ���� ����
		void doAttack();			// ���� ����
		void readyForAttackDelay(); // ���� �ĵ�
#pragma endregion



#pragma region Collision

#pragma endregion
		eBansheeState mMonsterActiveState;
#pragma region Chase Value
	private:
		float	mChaseDurationTime;			// �ִ� ���� �����ð�
		float	mChaseDurationDecay;		// ���� �����ð�
		float	mDistanceFromPlayer;		// �÷��̾���� �Ÿ�
#pragma endregion

#pragma region AttackCooldown condition value
		bool	mPrepareAttack;
		float	mPrepareDurationTime;
		float	mPrepareDurationDecay;
		bool	mAttackProgress;
		float	mReadyDurationTime;
		float	mReadyDurationDecay;
#pragma endregion

		bool	mIsAttacked;
	};
}