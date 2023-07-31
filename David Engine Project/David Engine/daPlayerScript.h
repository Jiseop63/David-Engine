#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daWeaponScript.h"

namespace da
{
	// ���� ����
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

		void PlayerInput();
		void DebugInput();
		void UIInput();
		void GetMouse();
		void CalcPlayerDir();
		void ReverseTexture();

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
		
		// �ӽ�
	public:
		void GetDamage();
		void GetHeal();

	public:
		math::Vector2 GetPlayerDir() { return mPlayerDir; }
		WeaponScript* SetWeaponObject(GameObject* object);
		EffectScript* SetEffectObject(GameObject* object);
	private:
		void timeProcess();
		void dashRegen();
		void jumpRegen();
		void bufferedJump();
		void jumpProcess();
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

		// �̰� ���Ŀ� ��ũ��Ʈ�� ���ؼ��� �����ҵ�
		WeaponScript*	mWeaponScript;
		EffectScript*	mEffectScript;

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