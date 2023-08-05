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
		void SetWeaponPosition(math::Vector3 vector3) { mWeaponTransform->SetPosition(vector3); }
		void SetWeaponRotation(math::Vector3 vector3) { mWeaponTransform->SetRotation(vector3); }
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }
		void ChangeWeapon(enums::eWeaponType weaponType);
		void DoAttack();

	private:
		void weaponInit(bool isMelee = true);
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
		structs::sInventory*				mInventoryData;
		std::shared_ptr<Texture>			mFirstTexture;
		std::shared_ptr<Texture>			mSecondTexture;

	private:
		enums::eWeaponType					mWeaponType;
		math::Vector2						mPlayerDir;
		bool								mAttackReady;
		bool								mWeaponAttacked;
		bool								mProjectileCollision;

		// 개선의 여지가 있음
		float	mEffectAngle;
		// 추후에 사라질 예정
		float	mAttackAccumulateTime;
		float	mAttackDelayTime;

	};
}