#include "daWeaponScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameDataManager.h"
#include "daEffectWeaponScript.h"
#include "daProjectileScript.h"
#include "daPlayerScript.h"

namespace da
{
	WeaponScript::WeaponScript()
		: mWeaponTransform(nullptr)
		, mWeaponRenderer(nullptr)
		, mWeaponAnimator(nullptr)
		, mPlayerScript(nullptr)
		, mEffects{}
		, mProjectiles{}
		, mActiveArmour(nullptr)
		, mWeaponTexture(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)

		, mEffectScale(math::Vector3::One)
		, mProjectileScale(math::Vector2::One)

		, mPlayerDir(math::Vector2::Zero)
		, mHitEffectAngle(0.0f)
		, mWeaponAttacked(false)
		, mEffectAngle(0.0f)
	{		
	}
	WeaponScript::~WeaponScript()
	{
	}
	void WeaponScript::Initialize()
	{
		// Tr세팅
		mWeaponTransform = GetOwner()->GetTransform();
		mWeaponTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// 근거리 무기는 Texture, 원거리 무기는 Animation 을 사용함
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();
		SetReverse(false);
		// 무기 세팅
		ChangeWeapon();
	}
	void WeaponScript::Update()
	{
		attackConditionCheck();
		projectileConditionCheck();
	}
		
#pragma region common func
	void WeaponScript::attackConditionCheck()
	{		
		if (!mActiveArmour->Weapon.AttackReady)
		{
			
			mActiveArmour->Weapon.AttackAccumulateTime += (float)Time::DeltaTime();

			if (mActiveArmour->Weapon.AttackDelayTime <= mActiveArmour->Weapon.AttackAccumulateTime)
			{			
				mActiveArmour->Weapon.AttackReady = true;
				// 충돌체 설정 해주기
				mActiveArmour->Weapon.AttackAccumulateTime = 0.0f;
			}
		}
	}
	void WeaponScript::projectileConditionCheck()
	{
		if (mActiveArmour)
		{
			if (mActiveArmour->Weapon.ProjectileCollision)
			{
				mActiveArmour->Weapon.ProjectileAccumulate += (float)Time::DeltaTime();
				if (mActiveArmour->Weapon.ProjectileValidTime <= mActiveArmour->Weapon.ProjectileAccumulate)
				{
					mActiveArmour->Weapon.ProjectileAccumulate = 0.0f;
					mActiveArmour->Weapon.ProjectileCollision = false;
				}
			}
		}
	}
#pragma endregion

#pragma region player Call
	void WeaponScript::SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir)
	{
		// Player 위치 저장
		mPlayerPosition = playerPos;

		// Player 방향 구하기
		mPlayerDir = playerDir;
		bool isLeft = false;
		if (0 >= playerDir.x)
			isLeft = true;

		// Weapon Rotate 구하기
		float angle = atan2(mPlayerDir.y, mPlayerDir.x);
		mEffectAngle = angle;

		// Weapon Position 구하기
		math::Vector2 armUp(-0.050f, 0.0f);
		math::Vector2 armDown(0.050f, -0.30f);
		math::Vector3 weaponPosistion = playerPos;

		// 공격할때마다 무기 회전값과 위치값 적용
		if (isLeft)
		{
			SetReverse(isLeft);
			if (!mWeaponAttacked)
			{
				angle += 2.710f;
				weaponPosistion.x += armUp.x;
				weaponPosistion.y += armDown.y;
			}
			else
			{
				angle -= 0.3930f;
				weaponPosistion.x += armDown.x;
			}
		}
		else
		{
			if (!mWeaponAttacked)
			{
				angle -= 2.710f;
				weaponPosistion.x -= armUp.x;
				weaponPosistion.y += armDown.y;

			}
			else
			{
				angle += 0.3930f;
				weaponPosistion.x -= armDown.x;
			}
		}
		weaponPosistion.x -= mPlayerDir.x * 0.050f;
		weaponPosistion.y -= mPlayerDir.y * 0.050f;
		// 변경된 값을 Weapon Transform에 적용하기
		mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mWeaponTransform->SetPosition(weaponPosistion);

	}
#pragma endregion
#pragma region Inventory Call
	void WeaponScript::ChangeWeapon()
	{
		// 기존 무기 초기화 해주기
		if (mActiveArmour)
			mActiveArmour->Weapon.AttackAccumulateTime = 0.0f;

		mActiveArmour = GameDataManager::GetActiveArmour();
		weaponInit();

	}
	void WeaponScript::weaponInit()
	{		
		if (mActiveArmour->Weapon.IsMeleeWeapon)
		{
			if (enums::eWeaponName::LongSword == mActiveArmour->Weapon.WeaponName)
			{
				// 무기 정보 세팅
				mActiveArmour->Weapon.AttackDelayTime = 0.450f;
				mActiveArmour->Weapon.AtaackDamage = 5.0f;
				mActiveArmour->Weapon.ProjectileValidTime = 0.250f;
				mActiveArmour->Weapon.ProjectileCenterPadding = 0.750f;
				mActiveArmour->Weapon.ProjectileRange = 3.5f;
				mProjectileScale = math::Vector2(2.50f, 1.750f);
				mWeaponTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
				mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");

				mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);
			}

			if (mWeaponTexture)
				mWeaponRenderer->ChangeSlotTexture(mWeaponTexture);
		}
	}
#pragma endregion
#pragma region Attack Process	
	void WeaponScript::DoAttack()
	{
		if (mActiveArmour->Weapon.AttackReady)
		{
			// 공격 활성화 (Tr 세팅, 이펙트, 충돌 객체 세팅)
			activeAttack();
			
			// 무기 텍스쳐 & 애니메이션 적용하기			
			playWeaponImage();
			
			mActiveArmour->Weapon.AttackReady = false;
		}
	}
	void WeaponScript::ActiveEffect()
	{
		// 방향 구하기
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		EffectWeaponScript* effect = callEffect();
		effect->SetEffectScale(mEffectScale);
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		effect->SetEffectPosition(mPlayerPosition + (playerDir * mActiveArmour->Weapon.ProjectileCenterPadding));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mActiveArmour->Weapon.WeaponName);
	}
	void WeaponScript::CallHitEffect(math::Vector3 position)
	{
		if (7 <= mHitEffectAngle)
			mHitEffectAngle = 0.0f;
		mHitEffectAngle += 1.80f;
		// 방향 구하기
		EffectWeaponScript* effect = callEffect();
		effect->SetEffectScale(math::Vector3(1.50f, 1.50f, 1.0f));
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mHitEffectAngle));
		effect->SetEffectPosition(position - math::Vector3(0.0f, 0.2f, 0.0f));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(L"Slash");
	}

	void WeaponScript::ActiveProjectile()
	{
		mActiveArmour->Weapon.ProjectileCollision = true;

		ProjectileScript* projectile = callProjectile();
		projectile->SetProjectileRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		projectile->SetProjectilePosition(mPlayerPosition);
		projectile->SetProjectileCenter(mPlayerDir * mActiveArmour->Weapon.ProjectileCenterPadding);
		projectile->SetProjectileSize(mProjectileScale);
		projectile->SetProjectileType(mActiveArmour->Weapon.ProjectileType);
		projectile->SetWeaponProjectile(mActiveArmour->Weapon);
		projectile->SetBeginProjectile(mPlayerPosition);
		projectile->GetOwner()->SetObjectState(GameObject::eObjectState::Active);

	}
	void WeaponScript::activeAttack()
	{
		// 이펙트 적용하기
		ActiveEffect();
		// 투사체 적용하기
		ActiveProjectile();
	}
	void WeaponScript::playWeaponImage()
	{
		if (mActiveArmour->Weapon.IsMeleeWeapon)
		{
			// Weapon Texture
			if (mWeaponAttacked)
				mWeaponAttacked = false;
			else
				mWeaponAttacked = true;
		}
		else
		{
			// Animation Play
		}
	}
#pragma endregion
#pragma region Effect Func
	void WeaponScript::AddEffectObject(GameObject* object)
	{
		EffectWeaponScript* weaponEffect = object->AddComponent<EffectWeaponScript>();
		weaponEffect->SetReqWeapon(this);
		mEffects.push_back(weaponEffect);
	}
	EffectWeaponScript* WeaponScript::callEffect()
	{
		for (size_t effect = 0; effect < mEffects.size(); effect++)
		{
			if (GameObject::eObjectState::Inactive ==
				mEffects[effect]->GetOwner()->GetObjectState())
				return mEffects[effect];
		}
		return nullptr;
	}
#pragma endregion
#pragma region Projectile Func
	void WeaponScript::AddProjectileObject(GameObject* object)
	{
		ProjectileScript* weaponProjectile = object->AddComponent<ProjectileScript>();
		weaponProjectile->SetReqWeapon(this);
		mProjectiles.push_back(weaponProjectile);
	}
	ProjectileScript* WeaponScript::callProjectile()
	{
		for (size_t projectile = 0; projectile < mProjectiles.size(); ++projectile)
		{
			if (GameObject::eObjectState::Inactive ==
				mProjectiles[projectile]->GetOwner()->GetObjectState())
				return mProjectiles[projectile];
		}
		return nullptr;
	}
#pragma endregion
}