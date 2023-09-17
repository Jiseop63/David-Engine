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

		// �ݵ�� ��ӹ޾Ƽ� �����ϼ���
		virtual void AddEffectObject(GameObject* object) {}
		virtual void AddProjectileObject(GameObject* object) {}

	protected:
		EffectScript* callEffect();
		ProjectileScript* callProjectile();
#pragma endregion

#pragma region call by player
	public:
		virtual void ToDoAttack() {}		// ����ؼ� ���弼��
#pragma endregion

#pragma region update Transform & Render & Condition
	protected:
		bool isLeft() { if (0 >= mOwnerDir.x) return true; else return false; }
		void updateReverseRenderer();
		virtual void updateWeaponPosition();
		virtual void updateWeaponRotate() {} // ����ؼ� ���弼��

		void updateAttackCoolDown(); // ���� ���� ������ ��ٿ� �����ϱ�
#pragma endregion

#pragma region Attack func (DoAttack ���� �����ϱ�)
	protected:
		// ���� ���� ������ ����Ʈ �� ����ü ȣ��
		virtual void attackEffect();				// ���� ����Ʈ ����		
		virtual void attackProjectile() {}	// ���� ����ü ����
		void attackPlay();					// ���� �̹��� ���
		
#pragma endregion
	public:
		void CallHitEffect(math::Vector3 position);		// �ǰ� ����� ȣ����
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