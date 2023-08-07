#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	class EffectWeaponScript;
	class ProjectileScript;
	class WeaponScript : public Script
	{
	public:
		WeaponScript();
		virtual ~WeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;

#pragma region Common Func
	public:
		void SetPlayerDir(math::Vector2 dir) { mPlayerDir = dir; }
		void ApplyProjectileCollision(bool value) { mProjectileCollision = value; }
		bool IsProjectileCollision() { return mProjectileCollision; }

	private:
		void attackConditionCheck();
		void calcWeaponAngle();
		void setTextures(std::shared_ptr<Texture> first, std::shared_ptr<Texture> second) { mFirstTexture = first; mSecondTexture = second; }
#pragma region Weapon Func
	public:
		void SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir);
		void SetWeaponPosition(math::Vector3 vector3) { mWeaponTransform->SetPosition(vector3); }
		void SetWeaponRotation(math::Vector3 vector3) { mWeaponTransform->SetRotation(vector3); }
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }
		void ChangeWeapon();
		void DoAttack();

	private:
		void weaponInit();
		void activeAttack();
		void playWeaponImage();
#pragma endregion
#pragma region Effect Func
	public:
		void AddEffectObject(GameObject* object);
		EffectWeaponScript* callEffect();
#pragma endregion
#pragma region Projectile Func
	public:
		void AddProjectileObject(GameObject* object);
		ProjectileScript* callProjectile();
#pragma endregion

	protected:
		Transform*							mWeaponTransform;
		Animator*							mWeaponAnimator;
		MeshRenderer*						mWeaponRenderer;
		std::vector<EffectWeaponScript*>	mEffects;
		std::vector<ProjectileScript*>		mProjectiles;

	protected:
		structs::sArmour*					mActiveArmour;
		std::shared_ptr<Texture>			mWeaponTexture;

		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;

	private:
		enums::eWeaponName					mWeaponType;
		math::Vector3						mPlayerPosition;
		math::Vector2						mPlayerDir;
		math::Vector2						mWeaponOffset;
		bool								mAttackReady;
		bool								mWeaponAttacked;
		// 이건 아직 용도를 모르겠음
		bool								mProjectileCollision;

	private:

		// 개선의 여지가 있음
		float	mEffectAngle;
	};
}