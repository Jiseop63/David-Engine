#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daWeaponScript.h"

namespace da
{
	// 라디안 방향
#define RW		1.570f
#define RNW		0.785f
#define RN		0
#define RNE		-0.785f
#define RE		-1.570f
#define RSE		-2.335f
#define RS		-3.140f
#define RSW		-3.925f

	enum class ePlayerState
	{
		Idle,
		Move,
		jumpProcess,
		Dead,
	};

	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize();
		virtual void Update();

		void GetMouse();
		void GetInput();
		void DebugInput();
		void UIInput();

		void PlayerFSM();
		void ChangeState(ePlayerState state);
		void HandleIdle();
		void HandleMove();
		void HandleJump();
		void HandleDead();


		void todoMove();
		void todoJump();
		void todoDash();
		void todoAttack();
		
		// 임시
	public:
		void GetDamage();
		void GetHeal();

	public:
		WeaponScript* SetWeaponObject(GameObject* object);
		
	private:
		void timeProcess();
		void dashRegen();
		void bufferedJump();
		void jumpProcess();
		void resetJumpBuffer();
	public:
		void InitAnimation();
		void InitCollider();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override {}
		virtual void OnCollisionStay(Collider2D* other) override {}
		virtual void OnCollisionExit(Collider2D* other) override {}
		virtual void OnLandEnter(Collider2D* other) override;
		virtual void OnLandStay(Collider2D* other) override;
		virtual void OnLandExit(Collider2D* other) override {}



		// component val
	protected:
		Transform*		mTransform;
		Rigidbody*		mRigidbody;
		Animator*		mAnimator;
		MeshRenderer*	mRenderer;
		
		Collider2D*		mBodyCollider;
		Collider2D*		mFootCollider;
		Collider2D*		mRightCollider;
		Collider2D*		mLeftCollider;

		// 이건 추후에 스크립트를 통해서만 제어할듯
		WeaponScript*	mWeaponScript;

		// data val
	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventoryData;

		// common val
	private:
		math::Vector2 mPlayerDir;

		// time val
	private:
		float	mDashAccumulateTime;
		float	mDashRegenTime;
		float	mJumpAccumulateTime;
		float	mJumpLimitTime;
		float	mAttackAccumulateTime;
		
		// func val
	private:
		float			mJumpForceRatio;
		bool			mAttacked;

		// condition val
	private:
		ePlayerState	mActiveState;
		int				mMoveCondition;
		bool			mBufferedJump;
		bool			mDead;


	};
}