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
		// Onwer가 무기를 세팅함
		void SetWeaponTexture(std::shared_ptr<Texture> texture) { mEnemyWeaponTexture = texture; }
		void SetWeaponTransform(math::Vector3 ownerPos, math::Vector2 ownerDir);

		// Onwer가 공격명령을 내림
		void DoAttack();
		// 애니메이션을 재생하고 콜라이더를 세팅함
		void activeAttack();
		// 방향에 따라 이미지 반전시켜주기
		void SetReverse(bool isLeft) { mEnemyWeaponRenderer->SetReverse(isLeft); }


	protected:
		Transform*		mEnemyWeaponTransform;
		MeshRenderer*	mEnemyWeaponRenderer;
		Animator*		mEnemyWeaponAnimator;

	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;
	};
}