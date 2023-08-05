#pragma once
#include "daScript.h"
#include "daWeaponScript.h"

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
		void turnOffCollision();


	public:
		// 콜라이더 세팅 (외부에서)
		void SetProjectilePosition(math::Vector3 vector3) { mProjectileTransform->SetPosition(vector3); }
		void SetProjectileRotation(math::Vector3 vector3) { mProjectileTransform->SetRotation(vector3); }
		void SetProjectileCollider(math::Vector2 size, math::Vector2 center);
		
		// 외부 세팅 함수
	public:
		void SetReqWeapon(WeaponScript* weapon) { mReqWeapon = weapon; }

	protected:
		Transform*	mProjectileTransform;
		Collider2D* mProjectileCollider;

	private:
		WeaponScript* mReqWeapon;
	};
}