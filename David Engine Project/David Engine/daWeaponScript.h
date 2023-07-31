#pragma once
#include "daScript.h"
#include "daAnimator.h"
#include "daMeshRenderer.h"
#include "daEffectScript.h"

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
		void SetWeaponPosition(math::Vector3 vector3) 
		{ 
			mWeaponTransform->SetPosition(vector3); 
			mEffectScript->SetEffectPosition(vector3);
		}
		void SetPlayerDir(math::Vector2 dir)
		{ 
			bool value = false;
			if (0 >= dir.x)
				value = true;
			float angle = atan2(dir.y, dir.x);
			mEffectScript->SetEffectRotation(math::Vector3(0.0f, 0.0f, angle));
			if (mWeaponAttacked)
			{
				if (value)
					angle += 0.7850f;
				else
					angle -= 0.7850f;
			}
			mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));

		}
		void SetWeaponRotation(math::Vector3 vector3) 
		{ 
			mWeaponTransform->SetRotation(vector3);
			mEffectScript->SetEffectRotation(vector3);
		}
		void SetWeaponRotation(float x, float y, float z)
		{
			if (mWeaponAttacked)
			{

			}

		}
		bool IsAttacked() { return mWeaponAttacked; }
		void SetReverse(bool value) { mWeaponRenderer->SetReverse(value); }
		void SetEffectObject(GameObject* object);

	protected:
		Transform*		mWeaponTransform;
		Animator*		mWeaponAnimator;
		MeshRenderer*	mWeaponRenderer;
		Collider2D*		mWeaponCollider;
		EffectScript*	mEffectScript;
		// 투사체 풀 만들기 (나중에)

	private:
		enums::eWeaponType	mWeaponType;

	protected:
		std::shared_ptr<Texture> mFirstTexture;
		std::shared_ptr<Texture> mSecondTexture;

	private:
		float	mAttackAccumulateTime;
		float	mAttackDelayTime;
		bool	mWeaponAttacked;

	};
}