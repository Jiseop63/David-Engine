#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"
namespace da
{
	enum class eHandFSM
	{
		Idle,
		Chase,
		Attack,
	};
	class PlayerScript;
	class SkellBossScript;
	class SkellBossHandScript : public Script
	{
	public:
		SkellBossHandScript();
		virtual ~SkellBossHandScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void SetOwnerScript(SkellBossScript* owner) { mOwnerScript = owner; }
		void SetLeftHand(bool left = true) { misLeftHand = left; }

		void DoAttack();
		bool IsAttackReady() { return mAttackReady; }
		bool IsAttackFinished() { return mAttackFinished; }
		void NotAttackFinish(bool value = false) { mAttackFinished = value; }
		void AttackIsFinished();

	private:
		void updateMoveToPlayer();

	private:
		bool isLeft() { if (misLeftHand) return true; else return false; }
		void reverseTexture() { mHandRenderer->SetReverse(!isLeft()); }

	private:
		void shootLaser();
	private:
		void retIdle();
		void activeCollider();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;


	private:
		Transform*		mHandTransform;
		MeshRenderer*	mHandRenderer;
		Animator*		mHandAnimator;
		Collider2D*		mHandCollider;
		PlayerScript*	mPlayerScript;
		eHandFSM		mHandState;

		SkellBossScript* mOwnerScript;

	private:
		bool			misLeftHand;
		float			mMovePositionY;

		bool			mChasePlayer;
		bool			mTargetFind;
		bool			mAttackReady;
		structs::sActionTimeValues mAttackDelay;
		structs::sActionTimeValues mAttackCooldown;

		bool			mAttackFinished;

	};
}