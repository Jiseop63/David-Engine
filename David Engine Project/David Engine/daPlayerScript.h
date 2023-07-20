#pragma once
#include "daScript.h"
#include "daRigidbody.h"
namespace da
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize();
		virtual void Update();

		void GetInput();
		void MoveFunc(math::Vector2 dir);
		void Dash();
		void Jump();
		// �ӽ�

		void GetDamage();
		void GetHeal();

		void regenDashCount();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionStay(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;


	protected:
		Rigidbody* mRigidbody;
		
	private:
		structs::sCreatureStat* mPlayerStat;
		structs::sDashCount* mDashCount;
		float mMoveSpeed;
		float mDashCountTime;
		float mRegenCountTime;
	};
}