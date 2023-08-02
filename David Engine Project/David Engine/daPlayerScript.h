#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daWeaponScript.h"
#include "daEffectPlayerScript.h"

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

		// 입력 함수
		void PlayerInput();
		void DebugInput();
		void UIInput();
		void GetMouse();
		void CalcPlayerDir();
		void ReverseTexture();


		// FSM 함수
		void ChangeState(ePlayerState state);
		void PlayerFSM();
		void HandleIdle();
		void HandleMove();
		void HandleJump();
		void HandleDead();


		// 행동 함수
		void todoMove();
		void todoJump();
		void todoDash();
		void todoAttack();
		EffectScript* findEffects();


		// 임시
	public:
		void GetDamage();
		void GetHeal();

		// 외부 변수
	public:
		math::Vector2 GetPlayerDir() { return mPlayerDir; }
		WeaponScript* SetWeaponObject(GameObject* object);
		EffectScript* AddEffectObject(GameObject* object);

		// 시간 관련 변수
	private:
		void timeProcess();
		void dashRegen();
		void jumpRegen();
		void bufferedJump();
		void jumpProcess();


		// 초기화
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

		// other script
	private:
		std::vector<EffectScript*> mEffects;
		// data val
	private:
		structs::sPlayerStat*	mPlayerStat;
		structs::sJumpCount*	mJumpCount;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventoryData;

		// common val
	private:
		math::Vector2 mPlayerDir;

		// condition val
	private:
		ePlayerState	mActiveState;
		int				mMoveCondition;
		bool			mDead;


	};
}