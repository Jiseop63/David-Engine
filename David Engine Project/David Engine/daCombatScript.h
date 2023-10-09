#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class CreatureScript;
	class CombatScript : public Script
	{
	public:
		CombatScript();
		virtual ~CombatScript();

		virtual void Initialize() override;

#pragma region Initialize & Get
	public:
		virtual void SetOwnerScript(CreatureScript* player) { mOwnerScript = player; }
		CreatureScript* GetOwnerScript() { return mOwnerScript; }
#pragma endregion
#pragma region update Transform & Render & Condition
	protected:
		bool isLeft() { if (0 >= mOwnerDir.x) return true; else return false; }
		void updateReverseRenderer();
		virtual void updateWeaponPosition();
		virtual void updateWeaponRotation() {} // 상속해서 만드세요

#pragma endregion
#pragma region Attack func
	public:
		virtual void ToDoAttack() {}		// 상속해서 만드세용
	protected:
		virtual void attackProjectile() {}	// 공격 투사체 세팅
#pragma endregion

	protected:
		Transform*						mCombatTransform;
		Animator*						mCombatAnimator;
		MeshRenderer*					mCombatRenderer;
		
	protected:
		CreatureScript*					mOwnerScript;
		math::Vector3					mOwnerPosition;
		math::Vector2					mOwnerDir;
		
	protected:
		float							mWeaponAngle;
		float							mHitEffectAngle;
		bool							mWeaponAttacked;
	};
}