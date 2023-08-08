#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	// WeaponScript -> CombatScript 로 변경할 것 !!
	class PlayerScript;
	class ProjectileScript;
	class EffectWeaponScript;
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

		void ModifyData();
		void ModifyProjectile(math::Vector2 size, float dirPadding, float validTime, enums::eProjectileType projectileType);
		void DoAttack();

	public:
		void ActiveEffect();
		void ActiveProjectile();

	private:
		void attackConditionCheck();
		void projectileConditionCheck();
		void activeAttack();

		
		void weaponInit();
		void playWeaponImage();
#pragma endregion

#pragma region Player Set
	public:
		void SetPlayerScript(PlayerScript* player) { mPlayerScript = player; }
		PlayerScript* GetPlayerScript() { return mPlayerScript; }
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
		PlayerScript*						mPlayerScript;
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
		// 외부에서 조건체크용으로 사용하는 변수
		bool								mProjectileCollision;
		float								mProjectileValidTime;
		float								mProjectileAccumulate;
		enums::eProjectileType				mProjectileType;
	private:

		// 개선의 여지가 있음
		float	mEffectAngle;
	};
}