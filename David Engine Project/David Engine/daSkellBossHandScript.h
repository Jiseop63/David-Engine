#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	enum class eHandFSM
	{
		Idle,
		Chase,
		Attack,
	};
	class Animator;
	class PlayerScript;
	class SkellBossHandScript : public Script
	{
	public:
		SkellBossHandScript();
		virtual ~SkellBossHandScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void SetLeftHand(bool left = true) { misLeftHand = left; }

		void DoAttack();


	private:
		void updateMoveToPlayer();

	private:
		bool isLeft() { if (misLeftHand) return true; else return false; }
		void reverseTexture() { mHandRenderer->SetReverse(!isLeft()); }

	private:
		void findPlayer();
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

	private:
		bool			misLeftHand;
		bool			mChasePlayer;
	private:
		float			mMovePositionY;
	};
}