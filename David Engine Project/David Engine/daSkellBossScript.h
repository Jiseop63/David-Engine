#pragma once
#include "daCreatureScript.h"
#include "daSkellBossHandScript.h"

namespace da
{
	class Animator;
	class PlayerScript;
	class SkellBossProjectileScript;	
	class SkellBossScript : public CreatureScript
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
		void SkellBossHandleIdle();					// 이건 솔직히 안씀
		void SkellBossHandleAttack();
		void SkellBossHandleDead();
#pragma endregion

#pragma region Attack Func
	private:
		void prepareForAttack();	// 공격 선딜
		void doAttack();			// 공격 진행
		void readyForAttackDelay(); // 공격 후딜

		void patternCondition();

		void callLaserAttack();
		void callProjectileAttack();
#pragma endregion

	public:
		Transform* GetBossTransform() { return mCreatureTransform; }
		void SetLeftHand(GameObject* left);
		void SetRightHand(GameObject* right);
		void IncreaseDamageCount() { ++mGetDamageCount; }



	public:
		virtual void AddActionUnit(GameObject* unit) override;
		SkellBossProjectileScript* CallBossProjectile();
		
	private:
		void retIdle();
		void attackingAnimation();



#pragma region Collision

#pragma endregion


	private:
		PlayerScript*	mPlayerScript;
		std::vector<SkellBossProjectileScript*>	mBossProjectiles;

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
		
		int			mPlayerJumpCount;
		int			mPlayerDashCount;
		int			mGetDamageCount;

		bool		mProjectileAttackOn;
		bool		mLaserAttackOn;
		float		mRotatePerSeconds;
		
		structs::sActionTimeValues mLaserCallDelayTime;
		structs::sActionTimeValues mProjectileCallDelayTime;

	};
}