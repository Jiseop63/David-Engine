#pragma once
#include "daCreatureScript.h"

namespace da
{
	class SkelScript : public CreatureScript
	{
	public:
		SkelScript();
		virtual ~SkelScript();

		virtual void Initialize() override;
		virtual void Update() override;

#pragma region Spawn
		// �ʱ� ��ġ�� ���� ���ϱ�
		// ���� ����Ʈ? �ִϸ��̼�? �����ϱ�
#pragma endregion

#pragma region Common Func
	private:
		void calcCreatureDir();
		void visualUpdate();
		void lifeCheck();
#pragma endregion

#pragma region FSM
	private:
		void ChangeState(eCreatureState state);
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



#pragma region Collision
		
#pragma endregion

#pragma region Chase Value
	private:
		float mChaseDurationTime;		// �ִ� ���� �����ð�
		float mChaseDurationDecay;		// ���� �����ð�
		float mDistanceFromPlayer;		// �÷��̾���� �Ÿ�
#pragma endregion

#pragma region AttackCooldown condition value
		bool			mPrepareAttack;
		float			mPrepareDurationTime;
		float			mPrepareDurationDecay;
		bool			mAttackProgress;
		float			mReadyDurationTime;
		float			mReadyDurationDecay;
#pragma endregion


	};
}