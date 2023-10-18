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
		void BossFSM();
		void SkellBossHandleIdle();
		void SkellBossHandleAttack();
		void SkellBossHandleDead();
#pragma endregion

#pragma region Attack Func
	private:
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
		SkellBossProjectileScript* CallBossActionUnit();
		void BossFindPlayer(bool value = true)
		{
			mPlayerFind = value;
			mBossOpeningCollider->ApplyComponentUsing(false);
		}

		//void SetBossLifeScript(LifeBarScript* lifeBar);

	private:
		void lifeCheck();

	private:
		void retIdle();
		void playAttackingAnimation();
		void startProjectileAttack();

	private:
		Collider2D*				mBossOpeningCollider;
		
		SkellBossHandScript*	mLeftHand;
		SkellBossHandScript*	mRightHand;
		std::vector<SkellBossProjectileScript*>	mBossProjectiles;

		PlayerScript*	mPlayerScript;

	private:


	private:
		eBossState					mBossActiveState;

		bool						mPlayerFind;

		structs::sActionTimeValues	mIdleStayTime;
		int							mGetDamageCount;

		bool						mAttacking;
		bool						mProjectileAttackActive;
		structs::sActionTimeValues	mProjectileCallDelayTime;
		structs::sActionTimeValues	mProjectileFinishTime;

		
		bool						mNextLaserAttackReady;		
		structs::sActionTimeValues	mLaserCallDelayTime;
		bool						mLaserAttakFinished;


	private:
		int				mPlayerJumpCount;
		int				mPlayerDashCount;
		bool			mProjectileAttackOn;
		float			mRotatePerSeconds;

		bool			mLeftHandTurn;
		int				mMaxLaserCount;
		int				mCurLaserCount;

		bool			mDeadTrigger;
	};
}