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
		bool IsCreatureAttackReady() { return mCreatureAttackReady; }



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
		bool			mCreatureAttackReady;
		float			mCreatureAttackAccumulateTime;
		float			mCreatureAttackDelayTime;
	};
}