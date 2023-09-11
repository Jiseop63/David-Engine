#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class Animator;
	class MonsterScript;
	class EnemyWeaponScript : public Script
	{
	public:
		EnemyWeaponScript();
		virtual ~EnemyWeaponScript();

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
		Collider2D*		mEnemyWeaponCollider;

		MonsterScript*	mMonsterScript;

	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;

	private:
		bool			mPlayerDamaged;
	};
}