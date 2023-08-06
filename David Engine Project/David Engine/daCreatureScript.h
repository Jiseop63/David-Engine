#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daEnemyWeaponScript.h"

namespace da
{
	enum class eCreatureState
	{
		Idle,
		Chase,
		Attack,
		Dead,
	};
	class CreatureScript : public Script
	{
	public:
		CreatureScript();
		virtual ~CreatureScript();

		virtual void Initialize();

		Transform* GetCreatureTransform() { return mTransform; }
		EnemyWeaponScript* SetEnemyWeaponScript(GameObject* creature);

	protected:
		Transform*	mTransform;
		Rigidbody*	mRigidbody;
		Animator*	mAnimator;
		Collider2D* mBodyCollider;
		Collider2D* mFootCollider;

	protected:
		EnemyWeaponScript* mWeaponScript;
	};
}