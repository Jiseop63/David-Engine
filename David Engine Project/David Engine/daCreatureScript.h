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

	public:
		Transform* GetCreatureTransform() { return mTransform; }
		EnemyWeaponScript* SetEnemyWeaponScript(GameObject* creature);

	protected:
		void OnDamaged();

#pragma region Components
	protected:
		Transform*	mTransform;
		Rigidbody*	mRigidbody;
		Animator*	mAnimator;
		Collider2D* mBodyCollider;
		Collider2D* mFootCollider;

		EnemyWeaponScript* mWeaponScript;
#pragma endregion
#pragma region Datas
	protected:
		//structs::sPlayerStat
#pragma endregion


	};
}