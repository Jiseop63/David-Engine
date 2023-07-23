#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
namespace da
{
	// 라디안 방향
#define RW		1.57f
#define RNW		0.785f
#define RN		0
#define RNE		-0.785f
#define RE		-1.57f
#define RSE		-2.335f
#define RS		-3.14f
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
		void WeaponMove();

	public:
		void GetDamage();
		void GetHeal();

	public:
		void SetWeaponObject(GameObject* object);
		
	private:
		void regenDashCount();
	public:
		void InitAnimation();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override {};
		virtual void OnCollisionStay(Collider2D* other) override {};
		virtual void OnCollisionExit(Collider2D* other) override {};


	protected:
		Transform*		mTransform;
		Rigidbody*		mRigidbody;
		Animator*		mAnimator;
		
		GameObject*		mWeaponObject;
		Transform*		mWeaponTransform;
		MeshRenderer*	mWeaponRenderer;

	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventory;

		// 임시 변수
	private:
		float	mMoveSpeed;
		float	mDashCountTime;
		float	mRegenCountTime;

		// 조건 변수
	private:
		int		mMoveCondition;
		bool	mReverse;
	};
}