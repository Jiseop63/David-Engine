#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class Animator;
	class EnemyWeaponScript : public Script
	{
	public:
		EnemyWeaponScript();
		virtual ~EnemyWeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;

	public:

	public:
		// Onwer�� ���⸦ ������
		void SetWeaponTexture(std::shared_ptr<Texture> texture) { mEnemyWeaponTexture = texture; }
		void SetWeaponTransform(math::Vector3 ownerPos) { mEnemyWeaponTransform->SetPosition(ownerPos); }

		// Onwer�� ���ݸ���� ����
		void DoAttack();
		// �ִϸ��̼��� ����ϰ� �ݶ��̴��� ������
		void activeAttack();
		// ���⿡ ���� �̹��� ���������ֱ�
		void SetReverse(bool isLeft) { mEnemyWeaponRenderer->SetReverse(isLeft); }


	public:
		virtual void OnCollisionEnter(Collider2D* other) override;


	protected:
		Transform*		mEnemyWeaponTransform;
		MeshRenderer*	mEnemyWeaponRenderer;
		Animator*		mEnemyWeaponAnimator;
		Collider2D*		mEnemyWeaponCollider;

	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;

	private:
		bool			mPlayerDamaged;
	};
}