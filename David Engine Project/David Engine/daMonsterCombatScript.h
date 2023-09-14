#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class Animator;
	class MonsterScript;
	class MonsterCombatScript : public Script
	{
	public:
		MonsterCombatScript();
		virtual ~MonsterCombatScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:		
		void SetOwnerScript(MonsterScript* target) { mMonsterScript = target; }

		// Onwer�� ���⸦ ������
		void SetWeaponTexture(std::shared_ptr<Texture> texture) { mEnemyWeaponTexture = texture; }
		void SetWeaponTransform(math::Vector3 ownerPos) { mEnemyWeaponTransform->SetPosition(ownerPos); }

		// Onwer�� ���ݸ���� ����
		void DoAttack();
		// 
		void activeAttack();
		void inactiveAttack();
		// ���⿡ ���� �̹��� ���������ֱ�
		void SetReverse(bool isLeft) { mEnemyWeaponRenderer->SetReverse(isLeft); }
		

	private:
		bool isLeft();
		void updateReverseRenderer() { bool value = isLeft(); mEnemyWeaponRenderer->SetReverse(value); }


	public:
		virtual void OnCollisionEnter(Collider2D* other) override;


	protected:
		Transform*		mEnemyWeaponTransform;
		MeshRenderer*	mEnemyWeaponRenderer;
		Animator*		mEnemyWeaponAnimator;
		Collider2D*		mEnemyWeaponCollider; // �浹ü ���� ����

		MonsterScript*	mMonsterScript; // owner
		// ����ü �߰��� ����
	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;

	private:
		bool			mPlayerDamaged;
	};
}