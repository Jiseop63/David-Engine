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

		// �ӽ�
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

		// �̰� ���Ŀ� ��ũ��Ʈ�� ���ؼ��� �����ҵ�
		GameObject*		mWeaponObject;
		Transform*		mWeaponTransform;
		MeshRenderer*	mWeaponRenderer;
		Collider2D*		mWeaponCollider;
		WeaponScript*	mWeaponScript;

	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* 	mDashCount;
		structs::sInventory*	mInventoryData;

		// �ӽ� ����
	private:
		float	mDashCountTime;
		float	mRegenCountTime;

		// ���� ����
	private:
		int		mMoveCondition;
		bool	mReverse;
		float   mAngle;
		bool	mAttacked;
	};
}