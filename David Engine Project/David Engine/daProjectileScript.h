#pragma once
#include "daScript.h"
	
namespace da
{
	class WeaponScript;
	class ProjectileScript : public Script
	{
	public:
		ProjectileScript();
		virtual ~ProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		void projectileProcess();
		void rangeProcess();
		void bodyProcess();

		void turnOffCollision();


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
		void SetReqWeapon(WeaponScript* weapon) { mReqWeapon = weapon; }

	protected:
		Transform*				mProjectileTransform;
		Collider2D*				mProjectileCollider;

	private:
		WeaponScript*			mReqWeapon;
		enums::eProjectileType	mProjectileType;
		math::Vector3			mProjectileDir;
		bool					mIsCollision;
	};
}