#include "daWeaponScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameDataManager.h"
#include "daEffectWeaponScript.h"
#include "daProjectileScript.h"
namespace da
{
	WeaponScript::WeaponScript()
		: mWeaponTransform(nullptr)
		, mWeaponRenderer(nullptr)
		, mWeaponAnimator(nullptr)
		, mEffects{}
		, mProjectiles{}
		, mActiveArmour(nullptr)
		, mWeaponTexture(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)

		, mWeaponType(enums::eWeaponName::LongSword)
		, mPlayerDir(math::Vector2::Zero)
		, mWeaponOffset(math::Vector2::Zero)
		, mAttackReady(true)
		, mWeaponAttacked(false)
		, mProjectileCollision(false)
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

		// 텍스쳐 (근거리에만 쓰임) 세팅
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		// 애니메이터 (원거리에만 쓰임) 세팅
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();

		// 무기 세팅
		ChangeWeapon();
	}
	void WeaponScript::Update()
	{
		// 공격 조건
		attackConditionCheck();
		// 무기 회전 (개편 예정)
		//calcWeaponAngle();
	}
		
#pragma region common func
	void WeaponScript::attackConditionCheck()
	{		
		if (!mAttackReady)
		{
			
			mActiveArmour->Weapon.AttackAccumulateTime += (float)Time::DeltaTime();

			if (mActiveArmour->Weapon.AttackDelayTime <= mActiveArmour->Weapon.AttackAccumulateTime)
			{			
				mAttackReady = true;
				// 충돌체 설정 해주기
				mActiveArmour->Weapon.AttackAccumulateTime = 0.0f;
			}
		}
	}
#pragma endregion

	void WeaponScript::calcWeaponAngle()
	{
		// 여기서 회전하는게 아니라 플레이어에 의해 회전이 적용되도록 변경할 예정임
				
		// 원본 회전값 저장
		float angle = atan2(mPlayerDir.y, mPlayerDir.x);
		mEffectAngle = angle;

		// 조금 변경된 각도로 무기 회전값 적용
		bool value = false;
		if (0 >= mPlayerDir.x)
			value = true;
		if (!mWeaponAttacked)
		{
			if (value)
				angle += 0.7850f;
			else
				angle -= 0.7850f;
		}
		mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
	}
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
			if (!mWeaponAttacked)
			{
				angle += 2.5710f;
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
				angle -= 2.5710f;
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
		switch (mActiveArmour->Weapon.WeaponName)
		{
		case da::enums::eWeaponName::Default:
			break;
		case da::enums::eWeaponName::LongSword:
			weaponInit();
			break;
		default:
			break;
		}
	}
	void WeaponScript::weaponInit()
	{
		
		if (mActiveArmour->Weapon.IsMeleeWeapon)
		{
			if (enums::eWeaponName::LongSword == mWeaponType)
			{
				// offset 세팅 / 플레이어 충돌체 크기 0.15+ 무기 텍스쳐 크기? 
				mWeaponOffset = math::Vector2(0.10f, -0.30f);
				mActiveArmour->Weapon.AttackDelayTime = 0.40f;
				mWeaponTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
				// Texture 세팅
				mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
				//setTextures(Resources::Find<Texture>(L"GreatSword1Texture"), Resources::Find<Texture>(L"GreatSword0Texture"));
				//mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
				
				// Effect 세팅
				
				// Collider 세팅

			}

			if (mWeaponTexture)
				mWeaponRenderer->ChangeSlotTexture(mWeaponTexture);
		}
	}
#pragma endregion
#pragma region Attack Process
	void WeaponScript::DoAttack()
	{
		if (mAttackReady)
		{
			// 공격 활성화 (Tr 세팅, 이펙트, 충돌 객체 세팅)
			activeAttack();
			
			// 무기 텍스쳐 & 애니메이션 적용하기			
			playWeaponImage();
			
			mAttackReady = false;
		}
	}
	void WeaponScript::activeAttack()
	{
		// 방향 구하기
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		// 이펙트 적용하기
		EffectWeaponScript* effect = callEffect();
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));		
		effect->SetEffectPosition(mPlayerPosition + (playerDir * 0.60f));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mWeaponType);

		// 투사체 적용하기
		ProjectileScript* projectile = callProjectile();
		projectile->SetProjectileRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		projectile->SetProjectilePosition(mPlayerPosition);
		projectile->SetProjectileCenter(mPlayerDir * 0.90f);
		projectile->SetProjectileSize(math::Vector2(2.50f, 1.50f));		
		projectile->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
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