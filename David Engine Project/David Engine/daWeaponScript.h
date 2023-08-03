#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daEffectWeaponScript.h"

namespace da
{
	class WeaponScript : public Script
	{
	public:
		WeaponScript();
		virtual ~WeaponScript();

		virtual void Initialize() override;
		virtual void Update() override;

		void SetTextures(std::shared_ptr<Texture> first, std::shared_ptr<Texture> second)
		{
			mFirstTexture = first; mSecondTexture = second;
		}


		void ChangeWeaponTexture();

		void AttackLogic();

		// 외부 기능 함수
	public:
		void DoAttack();
		void ChangeWeapon(enums::eWeaponType weaponType);
	private:
		void WeaponInit(bool isMelee = true);


		// 외부 함수
	public:
		void SetPlayerDir(math::Vector2 dir)
		{ 
			bool value = false;
			if (0 >= dir.x)
				value = true;
			float angle = atan2(dir.y, dir.x);
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
		void SetWeaponPosition(math::Vector3 vector3) 
		{ 
			mWeaponTransform->SetPosition(vector3); 
		}
		void SetWeaponRotation(math::Vector3 vector3) 
		{ 
			mWeaponTransform->SetRotation(vector3);
		}
		void SetWeaponRotation(float x, float y, float z)
		{
			if (mWeaponAttacked)
			{

			}

		}
		bool IsAttacked() { return mWeaponAttacked; }
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }

	public:
		void AddEffectObject(GameObject* object);
	private:
		EffectWeaponScript* callEffect();
		void ActiveEffect(EffectWeaponScript* effect, const std::wstring name);
	protected:
		Transform*		mWeaponTransform;
		Animator*		mWeaponAnimator;
		MeshRenderer*	mWeaponRenderer;
		Collider2D*		mWeaponCollider;
		std::vector<EffectWeaponScript*> mEffects;
	private:
		enums::eWeaponType	mWeaponType;

	protected:
		std::shared_ptr<Texture> mFirstTexture;
		std::shared_ptr<Texture> mSecondTexture;

	private:
		float	mAttackAccumulateTime;
		float	mAttackDelayTime;
		bool	mWeaponAttacked;

		float	mEffectAngle;

	};
}