#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class Animator;
	class SkellBossScript;
	class SkellBossProjectileScript : public Script
	{
	public:
		SkellBossProjectileScript();
		virtual ~SkellBossProjectileScript();

		virtual void Initialize() override;
		virtual void LateUpdate() override;

		// 외부에서 세팅
	public:
		void SetOwnerScript(SkellBossScript* owner) { mOwnerScript = owner; }
		
		void SetProjectileInfo(structs::sBossProjectileStat projectileStat) { *mProjectileInfo = projectileStat; }
		void SetProjectileSize(math::Vector2 size) { mProjectileCollider->SetSize(size); }	// 그냥 Tr에서 스케일 변경해도 될듯?

	public:
		void OnActive();			// 외부에서 활성화

	private:
		void playEffect(const std::wstring name);

		void rangeProcess();		// 투사체 로직
		void ClearProjectile();		// 변수 초기화
		void retInactive();			// 비활성화

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;

	private:
		Transform*						mProjectileTransform;
		MeshRenderer*					mProjectileRenderer;
		Animator*						mProjectileAnimator;
		Collider2D*						mProjectileCollider;
		SkellBossScript*				mOwnerScript;

	private:
		structs::sBossProjectileStat*	mProjectileInfo;
		math::Vector3					mBeginPosition;

	};
}