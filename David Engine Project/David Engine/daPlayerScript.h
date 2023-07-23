#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();

		void GetInput();
		void PlayAnimation();
		void MoveFunc(math::Vector2 dir);
		void Dash();
		void Jump();
		// �ӽ�

		void GetDamage();
		void GetHeal();

		void regenDashCount();

		void Complete();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;


	protected:
		Rigidbody* mRigidbody;
		Animator* mAnimator;

	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* 	mDashCount;

		// �ӽ� ����
	private:
		float	mMoveSpeed;
		float	mDashCountTime;
		float	mRegenCountTime;

		// ���� ����
	private:
		int		mMoveCondition;
		bool	mReverse;
	};
}