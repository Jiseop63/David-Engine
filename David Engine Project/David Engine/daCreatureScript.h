#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"
#include "daAnimator.h"

namespace da
{
	enum class eCreatureState
	{
		Idle,
		Chase,
		Attack,
		Dead,
	};
	class PlayerScript;
	class EnemyWeaponScript;
	class EffectEnemyScript;
	class CreatureLifebarScript;
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
		CreatureLifebarScript* SetCreatureLifeScript(GameObject* creature);
		EffectEnemyScript* AddEffectObject(GameObject* effectObject);
		structs::sCreatureStat GetCreatureStat() { return mCreatureStat; }

		void SetStandingPosition(math::Vector3 vector3) { mStandingPosition = vector3; }
		void SetDetectRange(float value) 
		{ 
			mCreatureStat.DetectRange = value; 
			mCreatureSensorCollider->SetSize(math::Vector2(value * 2.0f, 0.90f));
		}
		void SetAttackRange(float value) { mCreatureStat.AttackRange = value; }
#pragma endregion



#pragma region Collision Func
	public:
		virtual void OnCollisionEnter(Collider2D* other) {}
		virtual void OnCollisionExit(Collider2D* other) {}
		void OnDamaged();
		void IsFindPlayer(bool value) { mPlayerFind = value; }
#pragma endregion





#pragma region Components
	protected:
		Transform*				mCreatureTransform;
		MeshRenderer*			mCreatureRenderer;
		Rigidbody*				mCreatureRigidbody;
		Animator*				mCreatureAnimator;
		Collider2D*				mCreatureBodyCollider;
		Collider2D*				mCreatureFootCollider;
		Collider2D*				mCreatureSensorCollider;

		PlayerScript*			mPlayerScript;
		EnemyWeaponScript*		mCreatureWeaponScript;
		EffectEnemyScript*		mEnemyEffectScript;
		CreatureLifebarScript*	mCreatureLifeScript;

#pragma endregion
#pragma region Datas
	protected:
		structs::sCreatureStat	mCreatureStat;
#pragma endregion

#pragma region condition
	protected:
		eCreatureState			mCreatureActiveState;
		math::Vector3			mStandingPosition;
		math::Vector2			mCreatureDir;
		bool					mPlayerFind;
		bool					mIsDead;
#pragma endregion

	};
}