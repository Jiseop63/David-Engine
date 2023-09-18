#pragma once
#include "daScript.h"
#include "daSkellBossHandScript.h"

namespace da
{
	class Animator;
	class PlayerScript;
	class SkellBossScript : public Script
	{

		enum class eBossPattern
		{
			Barrage,
			OneWayLaser,
			ThreeWayLayer,
			SwardRain,
		};
		enum class eBossState
		{
			Idle,
			Attack,
			Dead,
		};
	public:
		SkellBossScript();
		virtual ~SkellBossScript();

		virtual void Initialize() override;
		virtual void Update() override;

#pragma region FSM
	private:
		void ChangeState(eBossState state);
		void BossFSM();
		void SkellBossHandleIdle();
		void SkellBossHandleAttack();
		void SkellBossHandleDead();
#pragma endregion
	public:
		Transform* GetBossTransform() { return mBossTransform; }
		void SetLeftHand(SkellBossHandScript* left) { mLeftHand = left; }
		void SetRightHand(SkellBossHandScript* right) { mRightHand = right; }

	private:
		void attackCoolDown();
		
#pragma region AttackPettern
		void shootLaser();

		// �� ������ ����				4�� ���� ���ع��� ������
		// �� ���� ������ ����		������ 3�� ���� ���ϸ�
		// ź�� ������ ����			������ 6ȸ �̻� �ϸ�
		// Į ����߸��� ����			��ø� 6�� �̻� ����ϸ�
		// ������� ���� �����Ǵ´�� ���� ����


		// ���� �����Ϸ��� Time���� ������ ������ ���� �ʿ��ҵ�?
#pragma endregion
	private:
		void retIdle();




#pragma region Collision

#pragma endregion


	private:
		Transform*		mBossTransform;
		Collider2D*		mBossCollider;
		Animator*		mBossAnimator;
		PlayerScript*	mPlayerScript;


	private:
		SkellBossHandScript*	mLeftHand;
		SkellBossHandScript*	mRightHand;
		bool					mLeftHandTurn;
		int						mMaxLaserCount;
		int						mCurLaserCount;

	private:
		eBossState		mBossActiveState;
		eBossPattern	mBossPattern;
		bool			mAttackReady;
		float			mAttackCoolDownDelayTime;
		float			mAttackCoolDownAccumulateTime;


		

		// ��ƼŬ ��ũ��Ʈ
		// ����Ʈ ��ũ��Ʈ
		// ����ü ��ũ��Ʈ
	};
}