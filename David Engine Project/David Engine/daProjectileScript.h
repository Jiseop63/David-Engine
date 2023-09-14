#pragma once
#include "daScript.h"
	
namespace da
{
	class CombatScript;
	class ProjectileScript : public Script
	{
	public:
		ProjectileScript();
		virtual ~ProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		void projectileProcess();
		void meleeProcess();
		void rangeProcess();
		void bodyProcess();

		void outOfTime();

	public:
		// 콜라이더 세팅 (외부에서)
		void SetProjectilePosition(math::Vector3 vector3) { mProjectileTransform->SetPosition(vector3); }
		void SetProjectileRotation(math::Vector3 vector3) { mProjectileTransform->SetRotation(vector3); }
		void SetProjectileDir(math::Vector3 vector3) { mProjectileDir = vector3; }
		void SetProjectileSize(math::Vector2 vector2) { mProjectileCollider->SetSize(vector2); }
		void SetProjectileCenter(math::Vector2 center) { mProjectileCollider->SetCenter(center); }
		void SetProjectileType(enums::eProjectileType type) { mProjectileType = type; }

		// 외부 세팅 함수
	public:
		void SetWeaponScript(CombatScript* weapon) { mWeaponScript = weapon; }
		CombatScript* GetWeaponScript() { return mWeaponScript; }
		void SetWeaponProjectile(structs::sWeaponInfo weaponInfo) 
		{ 
			mWeaponInfo = weaponInfo;
			mWeaponInfo.ProjectileStat.ProjectileActive = true;
		}
		void SetBeginProjectile(math::Vector3 vector3) { mBeginPosition = vector3; }

		void ClearProjectile();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;

	protected:
		Transform*				mProjectileTransform;
		Collider2D*				mProjectileCollider;
		Transform*				mWeaponTransform;

	private:
		CombatScript*			mWeaponScript;
		structs::sWeaponInfo	mWeaponInfo;

		enums::eProjectileType	mProjectileType;
		math::Vector3			mProjectileDir;
		math::Vector3			mBeginPosition;
		bool					mIsCollision;
	};
}