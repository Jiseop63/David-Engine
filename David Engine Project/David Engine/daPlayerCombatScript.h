#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	// WeaponScript -> CombatScript �� ������ �� !!
	class PlayerScript;
	class ProjectileScript;
	class EffectScript;
	class CombatScript : public Script
	{
	public:
		CombatScript();
		virtual ~CombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

#pragma region Initialize & Get
	public:
		void SetPlayerScript(PlayerScript* player) { mPlayerScript = player; }
		PlayerScript* GetPlayerScript() { return mPlayerScript; }
		
		void AddEffectObject(GameObject* object);
		void AddProjectileObject(GameObject* object);

	private:
		EffectScript* callEffect();
		ProjectileScript* callProjectile();
#pragma endregion

#pragma region call by player
	public:
		void EquipWeapon(); 							// ���� ��ü�� �κ��丮�� ���ؼ� ȣ���
		
		void ToDoAttack();								// playerScript���� ȣ��
		void CallHitEffect(math::Vector3 position);		// �ǰ� ����� ȣ����
#pragma endregion

#pragma region update Transform & Render & Condition
	private:
		// ������ ���� ���ϱ�
		bool IsLeft() { if (0 >= mPlayerDir.x) return true; else return false; }	// ������ ��� ����?

		// update ���� Transform �� render ����
		void updateWeaponPosition();			// Update���� �׻� �÷��̾� ��ġ�� �̵�
		void updateWeaponRotate();				// Update���� �׻� ���콺 �������� ȸ����
		void updateReverseRenderer() { bool value = IsLeft(); mWeaponRenderer->SetReverse(value); mWeaponRenderer->SetReverse(value); }

		void updateAttackCoolDown(); // ���� ���� ������ ��ٿ� �����ϱ�
#pragma endregion

#pragma region Attack func
	private:
		// ���� �ִϸ��̼� ����	*�ٰŸ��� bool ���� �ϳ��� �ΰ� �̹����� ��ü, ���Ÿ��� �ִϸ��̼� ���
		void playWeaponAttack();

		// ���� ���� ������ ����Ʈ �� ����ü ȣ��
		void attackEffect();			// ���� ����Ʈ ����		
		void attackProjectile();		// ���� ����ü ����
		void attackAnimation();			// ���� �̹��� �������ִ� �Լ�

#pragma endregion

	protected:
		Transform*							mWeaponTransform;
		Animator*							mWeaponAnimator;
		MeshRenderer*						mWeaponRenderer;
		PlayerScript*						mPlayerScript;
		std::vector<EffectScript*>			mEffects;
		std::vector<ProjectileScript*>		mProjectiles;

	protected:
		structs::sArmour*					mActiveArmour;
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;

	private:
		std::wstring mEffectName;


	private:
		math::Vector3						mEffectScale;
		math::Vector2						mProjectileScale;
		math::Vector3						mPlayerPosition;
		math::Vector2						mPlayerDir;
		float								mHitEffectAngle;
		bool								mWeaponAttacked;

	private:

		// ������ ������ ����
		float	mEffectAngle;
	};
}