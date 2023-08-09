#pragma once
#include "daScript.h"
#include "daRigidbody.h"
#include "daAnimator.h"
#include "daEnemyWeaponScript.h"
#include "daEffectEnemyScript.h"
#include "daPlayerScript.h"

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
#pragma region common Func
		void ReverseTexture();
		bool IsLeft() { if (0 >= mCreatureDir.x) return true; else return false; }
#pragma endregion


#pragma region public Func
	public:
		Transform* GetCreatureTransform() { return mCreatureTransform; }
		EnemyWeaponScript* SetEnemyWeaponScript(GameObject* creature);
		EffectEnemyScript* AddEffectObject(GameObject* effectObject);
#pragma endregion

	public:
		void OnDamaged();

#pragma region Components
	protected:
		Transform*			mCreatureTransform;
		MeshRenderer*		mCreatureRenderer;
		Rigidbody*			mCreatureRigidbody;
		Animator*			mCreatureAnimator;
		Collider2D*			mCreatureBodyCollider;
		Collider2D*			mCreatureFootCollider;

		EnemyWeaponScript*	mCreatureWeaponScript;
		EffectEnemyScript*	mEnemyEffectScript;
		PlayerScript*		mPlayerScript;


#pragma endregion
#pragma region Datas
	protected:
		structs::sCreatureStat mCreatureStat;
#pragma endregion

#pragma region condition
	protected:
		math::Vector2	mCreatureDir;
		eCreatureState	mCreatureActiveState;
		bool			mIsDead;
#pragma endregion

	};
}