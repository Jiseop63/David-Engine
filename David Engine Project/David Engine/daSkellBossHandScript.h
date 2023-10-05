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
		bool			mChasePlayer;
		bool			mAttackAble;
	private:
		float			mMovePositionY;
		structs::sActionTimeValues mAttackDelay;
	};
}