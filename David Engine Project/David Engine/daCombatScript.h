#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class CreatureScript;
	class ProjectileScript;
	class EffectScript;
	class CombatScript : public Script
	{
	public:
		CombatScript();
		virtual ~CombatScript();

		virtual void Initialize() override;

#pragma region Initialize & Get
	public:
		virtual void AddOwnerScript(CreatureScript* player) { mOwnerScript = player; }
		CreatureScript* GetOwnerScript() { return mOwnerScript; }

		// 반드시 상속받아서 구현하세요
		virtual void AddEffectObject(GameObject* object) {}
		virtual void AddProjectileObject(GameObject* object) {}

	protected:
		EffectScript* callEffect();
		ProjectileScript* callProjectile();
#pragma endregion

#pragma region call by player
	public:
		virtual void ToDoAttack() {}		// 상속해서 만드세용
#pragma endregion

#pragma region update Transform & Render & Condition
	protected:
		bool isLeft() { if (0 >= mOwnerDir.x) return true; else return false; }
		void updateReverseRenderer();
		virtual void updateWeaponPosition();
		virtual void updateWeaponRotate() {} // 상속해서 만드세요

		void updateAttackCoolDown(); // 공격 스텟 정보로 쿨다운 적용하기
#pragma endregion

#pragma region Attack func (DoAttack 에서 구현하기)
	protected:
		// 공격 스텟 정보로 이펙트 및 투사체 호출
		virtual void attackEffect();				// 공격 이펙트 세팅		
		virtual void attackProjectile() {}	// 공격 투사체 세팅
		void attackPlay();					// 무기 이미지 재생
		
#pragma endregion
	public:
		void CallHitEffect(math::Vector3 position);		// 피격 대상이 호출함
	protected:
		Transform*						mCombatTransform;
		Animator*						mCombatAnimator;
		MeshRenderer*					mCombatRenderer;
		
		std::vector<EffectScript*>		mCombatEffects;
		std::vector<ProjectileScript*>	mCombatProjectiles;

	protected:
		CreatureScript*					mOwnerScript;
		math::Vector3					mOwnerPosition;
		math::Vector2					mOwnerDir;
		structs::sWeaponInfo*			mWeaponInfo;

	protected:
		math::Vector3					mEffectScale;
		math::Vector2					mProjectileSize;
		
	protected:
		float							mEffectAngle;
		float							mHitEffectAngle;
		bool							mWeaponAttacked;
	};
}