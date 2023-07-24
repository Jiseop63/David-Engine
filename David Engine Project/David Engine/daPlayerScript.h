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

	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize();
		virtual void Update();

		void GetInput();
		void GetMouse();
		void MoveFunc(math::Vector2 dir);
		void Dash();
		void Jump();

		// 임시
	public:
		void GetDamage();
		void GetHeal();

	public:
		void SetWeaponObject(GameObject* object);
		
	private:
		void regenDashCount();
	public:
		void InitAnimation();
		void InitCollider();
	public:
		virtual void OnCollisionEnter(Collider2D* other) override {};
		virtual void OnCollisionStay(Collider2D* other) override {};
		virtual void OnCollisionExit(Collider2D* other) override {};


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
		GameObject*		mWeaponObject;
		Transform*		mWeaponTransform;
		MeshRenderer*	mWeaponRenderer;
		Collider2D*		mWeaponCollider;
		WeaponScript*	mWeaponScript;

	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventoryData;

		// 임시 변수
	private:
		float	mDashCountTime;
		float	mRegenCountTime;

		// 조건 변수
	private:
		int		mMoveCondition;
		bool	mReverse;
		float   mAngle;
		bool	mAttacked;
	};
}