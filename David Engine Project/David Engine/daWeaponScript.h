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

#pragma region Weapon Func
	public:
		void SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir);
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }
		void ChangeWeapon();
		void DoAttack();

	private:
		void attackConditionCheck();
		void activeAttack();
		void weaponInit();
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
		void ApplyProjectileCollision(bool value) { mProjectileCollision = value; }
		bool IsProjectileCollision() { return mProjectileCollision; }
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
		bool								mAttackReady;
		bool								mWeaponAttacked;

		math::Vector2						mProjectileSize;
		float								mProjectileCenterPadding;
		// 이건 아직 용도를 모르겠음
		bool								mProjectileCollision;

	private:

		// 개선의 여지가 있음
		float	mEffectAngle;
	};
}