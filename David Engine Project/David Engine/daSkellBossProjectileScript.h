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

		// �ܺο��� ����
	public:
		void SetOwnerScript(SkellBossScript* owner) { mOwnerScript = owner; }
		
		void SetProjectileInfo(structs::sBossProjectileStat projectileStat) { *mProjectileInfo = projectileStat; }
		void SetProjectileSize(math::Vector2 size) { mProjectileCollider->SetSize(size); }	// �׳� Tr���� ������ �����ص� �ɵ�?

	public:
		void OnActive();			// �ܺο��� Ȱ��ȭ

	private:
		void playEffect(const std::wstring name);

		void rangeProcess();		// ����ü ����
		void ClearProjectile();		// ���� �ʱ�ȭ
		void retInactive();			// ��Ȱ��ȭ

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