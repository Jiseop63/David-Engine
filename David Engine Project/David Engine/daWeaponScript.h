#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daEffectWeaponScript.h"
#include "daProjectileScript.h"
namespace da
{
	class WeaponScript : public Script
	{
	public:
		WeaponScript();
		virtual ~WeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;


	public:
		void attackConditionCheck();
		void calcWeaponAngle();
	public:
		void playWeaponImage();
		void setTextures(std::shared_ptr<Texture> first, std::shared_ptr<Texture> second) { mFirstTexture = first; mSecondTexture = second; }

		// 로직 함수
	private:
		void activeAttack();


		// 외부 기능 함수
	public:
		void DoAttack();
		void ChangeWeapon(enums::eWeaponType weaponType);


		// 초기화 함수
	private:
		void WeaponInit(bool isMelee = true);


		// 외부 함수
	public:
		void SetPlayerDir(math::Vector2 dir)
		{
			mPlayerDir.x = dir.x;
			mPlayerDir.y = dir.y;
		}		
		void SetWeaponPosition(math::Vector3 vector3) { mWeaponTransform->SetPosition(vector3); }
		void SetWeaponRotation(math::Vector3 vector3) { mWeaponTransform->SetRotation(vector3); }
		bool IsAttacked() { return mWeaponAttacked; }
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }

	public:
		void AddEffectObject(GameObject* object);
		void AddProjectileObject(GameObject* object);
	private:
		EffectWeaponScript* callEffect();
		ProjectileScript* callProjectile();
		void ActiveEffect(EffectWeaponScript* effect, const std::wstring name);


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
		math::Vector4						mPlayerDir;
		bool								mAttackReady;
		bool								mWeaponAttacked;

		// 개선의 여지가 있음
		float	mEffectAngle;
		// 추후에 사라질 예정
		float	mAttackAccumulateTime;
		float	mAttackDelayTime;

	};
}