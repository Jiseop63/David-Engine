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

		// Onwer가 무기를 세팅함
		void SetWeaponTexture(std::shared_ptr<Texture> texture) { mEnemyWeaponTexture = texture; }
		void SetWeaponTransform(math::Vector3 ownerPos) { mEnemyWeaponTransform->SetPosition(ownerPos); }

		// Onwer가 공격명령을 내림
		void DoAttack();
		// 
		void activeAttack();
		void inactiveAttack();
		// 방향에 따라 이미지 반전시켜주기
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
		Collider2D*		mEnemyWeaponCollider; // 충돌체 없앨 예정

		MonsterScript*	mMonsterScript; // owner
		// 투사체 추가할 예정
	protected:
		std::shared_ptr<Texture> mEnemyWeaponTexture;

	private:
		bool			mPlayerDamaged;
	};
}