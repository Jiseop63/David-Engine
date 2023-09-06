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
		// GameData �� ���� ���� ���� �������ֱ� (����, ���� ��ü�� ȣ��) ? ���� ��ü�� ���� ȣ�����ֳ�? GameDataManager? Player? 
		void EquipWeapon() {}					// gameData ���� ���� �������� ���� enum ���� �������� ���õ�
		// �÷��̾ �����Ҷ� ȣ��
		//void DoAttack() {}
#pragma endregion

#pragma region update Transform & Render & Condition
	private:
		// ������ ���� ���ϱ�
		bool IsLeft() { /*if (0 >= mCreatureDir.x) return true; else return false;*/ }

		// update ���� Transform �� render ����
		void updateWeaponPosition() {}			// Update���� �׻� �÷��̾� ��ġ�� �̵�
		void updateWeaponRotate() {}			// Update���� �׻� ���콺 �������� ȸ����
		void updateReverseRenderer() { /*bool value = IsLeft(); mCreatureRenderer->SetReverse(value); mCreatureWeaponScript->SetReverse(value);*/ }

		// ���� ���� ������ ��ٿ� �����ϱ�
		void updateAttackCoolDown() {};
#pragma endregion

#pragma region Attack func
	private:
		// ���� �ִϸ��̼� ����	*�ٰŸ��� bool ���� �ϳ��� �ΰ� �̹����� ��ü, ���Ÿ��� �ִϸ��̼� ���
		void playWeaponAttack() {}

		// ���� ���� ������ ����Ʈ �� ����ü ȣ��
		void attackEffect() {}			// ���� ����Ʈ ����		
		void attackProjectile() {}		// ���� ����ü ����
#pragma endregion

#pragma region Weapon Func
	public:
		void CallHitEffect(math::Vector3 position);										// Ÿ�� ����Ʈ ȣ���� ���� �Լ�
		void SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir);		// ���� ��ġ�� ������ ������ (����)
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }				// �̹��� ���� �Լ�
		void ChangeWeapon();															// *�Ⱦ�

		void DoAttack();																// �÷��̾ �����Ҷ� ȣ���ϴ� �Լ�

	public:
		void ActiveEffect();															// ���ݽ� ����Ʈ ȣ��
		void ActiveProjectile();														// ���ݽ� ����ü ȣ��

	private:
		void attackConditionCheck();													// �Ƹ� ��ٿ� �Լ��ε�
		void projectileConditionCheck();												// �Ƹ� ����ü �����ð��ε�
		void activeAttack();															// �Ⱦ�

		
		void weaponInit();																// �������� �����ϴ� �Լ�
		void playWeaponImage();															// ���� �̹��� �������ִ� �Լ�, �����Ҷ����� ȣ��
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