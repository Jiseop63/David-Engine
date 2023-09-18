#pragma once
#include "daScript.h"
#include "daSkellBossHandScript.h"

namespace da
{
	class Animator;
	class PlayerScript;
	class SkellBossProjectileScript;	
	class SkellBossScript : public Script
	{
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
		void ChangeStateAnimation(eBossState state);
		void BossFSM();
		void SkellBossHandleIdle();					// �̰� ������ �Ⱦ�
		void SkellBossHandleAttack();
		void SkellBossHandleDead();
#pragma endregion

#pragma region Attack Func
	private:
		void prepareForAttack();	// ���� ����
		void doAttack();			// ���� ����
		void readyForAttackDelay(); // ���� �ĵ�

		void patternCondition();

		void callLaserAttack();
		void callProjectileAttack();
#pragma endregion

	public:
		Transform* GetBossTransform() { return mBossTransform; }
		void SetLeftHand(SkellBossHandScript* left) { mLeftHand = left; }
		void SetRightHand(SkellBossHandScript* right) { mRightHand = right; }
		void IncreaseDamageCount() { ++mGetDamageCount; }
		void AddProjectileObject(GameObject* projectile);


	private:
		SkellBossProjectileScript* callProjectile();
		
	private:
		void retIdle();
		void attackingAnimation();



#pragma region Collision

#pragma endregion


	private:
		Transform*		mBossTransform;
		Collider2D*		mBossCollider;
		Animator*		mBossAnimator;
		PlayerScript*	mPlayerScript;


	private:
		std::vector<SkellBossProjectileScript*>	mBossProjectiles;

		structs::sBossProjectileStat*	mBossProjectileInfo;
		math::Vector2					mBossProjectileSize;


	private:
		SkellBossHandScript*	mLeftHand;
		SkellBossHandScript*	mRightHand;
		bool					mLeftHandTurn;
		int						mMaxLaserCount;
		int						mCurLaserCount;

	private:
		eBossState		mBossActiveState;
		bool			mAttackReady;
		float			mAttackCoolDownDelayTime;
		float			mAttackCoolDownAccumulateTime;

		bool			mPrepareAttack;
		float			mPrepareDurationTime;
		float			mPrepareDurationDecay;
		bool			mAttackProgress;
		float			mReadyDurationTime;
		float			mReadyDurationDecay;
		


		int		mPlayerJumpCount;
		int		mPlayerDashCount;
		int		mGetDamageCount;


		bool	mProjectileAttackOn;
		bool	mLaserAttackOn;

		float	mLaserCallDelayTime;
		float	mLaserCallDelayDecay;
		float	mProjectileCallDelayTime;
		float	mProjectileCallDelayDecay;

		float	mRotatePerSeconds;
	};
}