#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	class EnemyWeaponScript : public Script
	{
	public:
		EnemyWeaponScript();
		virtual ~EnemyWeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;

	public:
		// Onwer�� ���⸦ ������
		void SetWeaponTexture(std::shared_ptr<Texture> texture) { mEnemyWeaponTexture = texture; }
		void SetWeaponTransform(math::Vector3 ownerPos, math::Vector2 ownerDir);

		// Onwer�� ���ݸ���� ����
		void DoAttack();
		// �ִϸ��̼��� ����ϰ� �ݶ��̴��� ������
		void activeAttack();
		// ���⿡ ���� �̹��� ���������ֱ�
		void SetReverse(bool isLeft) { mEnemyWeaponRenderer->SetReverse(isLeft); }


	protected:
		Transform*		mEnemyWeaponTransform;
		MeshRenderer*	mEnemyWeaponRenderer;
		Animator*		mEnemyWeaponAnimator;

	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;
	};
}