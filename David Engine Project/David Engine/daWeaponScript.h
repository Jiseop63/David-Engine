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

	public:
		void playWeaponImage();
		void setTextures(std::shared_ptr<Texture> first, std::shared_ptr<Texture> second) { mFirstTexture = first; mSecondTexture = second; }


		// �ܺ� ��� �Լ�
	public:
		void DoAttack();
		void ChangeWeapon(enums::eWeaponType weaponType);


		// �ʱ�ȭ �Լ�
	private:
		void WeaponInit(bool isMelee = true);


		// �ܺ� �Լ�
	public:
		void SetPlayerDir(math::Vector2 dir)
		{
			mPlayerDir.x = dir.x;
			mPlayerDir.y = dir.y;
		}
		void calcWeaponAngle()
		{
			// ��, �� ���⿡ ���� angle ���
			bool value = false;
			if (0 >= mPlayerDir.x)
				value = true;
			float angle = atan2(mPlayerDir.y, mPlayerDir.x);
			mEffectAngle = angle;

			if (mWeaponAttacked)
			{
				if (value)
					angle += 0.7850f;
				else
					angle -= 0.7850f;
			}
			mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		}
		void initWeaponRotate()
		{
			float angle = atan2(mPlayerDir.y, mPlayerDir.x);
			mEffectAngle = angle;

			if (mWeaponAttacked)
			{
				if (value)
					angle += 0.7850f;
				else
					angle -= 0.7850f;
			}
			mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
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

		// ������ ������ ����
		float	mEffectAngle;
		// ���Ŀ� ����� ����
		float	mAttackAccumulateTime;
		float	mAttackDelayTime;

	};
}