#pragma once
#include "daMonsterScript.h"

namespace da
{
	enum class eSkulState
	{
		Idle,
		Chase,
		Attack,
		Dead,
	};

	class SkelScript : public MonsterScript
	{
	public:
		SkelScript();
		virtual ~SkelScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		CombatScript* AddCombatObject(GameObject* object);
		
#pragma region Spawn
#pragma endregion

#pragma region Common Func
	private:
		void calcTargetDir();
		void lifeCheck();
#pragma endregion

#pragma region FSM
	private:
		void ChangeState(eSkulState state);
		void skelFSM();
		void SkelHandleIdle();
		void SkelHandleChase();
		void SkelHandleAttack();
		void SkelHandleDead();
#pragma endregion

#pragma region Chase Func
	private:
		void trackingPlayer();		// �÷��̾ ���ؼ� �̵��ϱ�
		void findingPlayer();		// �߰� ���ѷ���
#pragma endregion

#pragma region Attack Func
	private:
		void prepareForAttack();	// ���� ����
		void doAttack();			// ���� ����
		void readyForAttackDelay(); // ���� �ĵ�
#pragma endregion

	public:

		eSkulState				mMonsterActiveState;
		
#pragma region Chase Value
	private:
		float	mChaseDurationTime;		// �ִ� ���� �����ð�
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
	};
}