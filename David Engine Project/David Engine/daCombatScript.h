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
		virtual void updateWeaponRotate() {} // 상속해서 만드세요

		void updateAttackCoolDown(); // 공격 스텟 정보로 쿨다운 적용하기
#pragma endregion
#pragma region Attack func
	public:
		virtual void ToDoAttack() {}		// 상속해서 만드세용
	protected:
		virtual void attackProjectile() {}	// 공격 투사체 세팅
		void attackPlay();					// 무기 이미지 재생		
#pragma endregion

	protected:
		Transform*						mCombatTransform;
		Animator*						mCombatAnimator;
		MeshRenderer*					mCombatRenderer;
		
	protected:
		CreatureScript*					mOwnerScript;
		math::Vector3					mOwnerPosition;
		math::Vector2					mOwnerDir;
		structs::sWeaponInfo*			mWeaponInfo;

	protected:
		math::Vector3					mEffectScale;		// 개편 필요함
		math::Vector2					mProjectileSize;	// 개편 필요함
		
	protected:
		float							mEffectAngle;
		float							mHitEffectAngle;
		bool							mWeaponAttacked;
	};
}