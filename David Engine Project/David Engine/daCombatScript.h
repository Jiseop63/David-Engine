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
		virtual void updateWeaponRotation() {} // ����ؼ� ���弼��

#pragma endregion
#pragma region Attack func
	public:
		virtual void ToDoAttack() {}		// ����ؼ� ���弼��
	protected:
		virtual void attackProjectile() {}	// ���� ����ü ����
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