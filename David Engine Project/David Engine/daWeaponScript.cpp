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
		, mEffects{}
		, mProjectiles{}
		, mActiveArmour(nullptr)
		, mWeaponTexture(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)

		, mWeaponType(enums::eWeaponName::LongSword)
		, mPlayerDir(math::Vector2::Zero)
		, mAttackReady(true)
		, mWeaponAttacked(false)
		, mProjectileSize(math::Vector2::Zero)
		, mProjectileCenterPadding(0.0f)

		, mProjectileType(enums::eProjectileType::Melee)
		, mProjectileCollision(false)
		, mProjectileValidTime(0.0f)
		, mProjectileAccumulate(0.0f)
		, mEffectAngle(0.0f)
	{		
	}
	WeaponScript::~WeaponScript()
	{
	}
	void WeaponScript::Initialize()
	{
		// Tr����
		mWeaponTransform = GetOwner()->GetTransform();
		mWeaponTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// �ؽ��� (�ٰŸ����� ����) ����
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		// �ִϸ����� (���Ÿ����� ����) ����
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();
		SetReverse(false);
		// ���� ����
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
		if (!mAttackReady)
		{
			
			mActiveArmour->Weapon.AttackAccumulateTime += (float)Time::DeltaTime();

			if (mActiveArmour->Weapon.AttackDelayTime <= mActiveArmour->Weapon.AttackAccumulateTime)
			{			
				mAttackReady = true;
				// �浹ü ���� ���ֱ�
				mActiveArmour->Weapon.AttackAccumulateTime = 0.0f;
			}
		}
	}
	void WeaponScript::projectileConditionCheck()
	{
		if (mProjectileCollision)
		{
			mProjectileAccumulate += (float)Time::DeltaTime();
			if (mProjectileValidTime <= mProjectileAccumulate)
			{
				mProjectileAccumulate = 0.0f;
				mProjectileCollision = false;
			}
		}
	}
#pragma endregion

#pragma region player Call
	void WeaponScript::SetWeaponTransform(math::Vector3 playerPos, math::Vector2 playerDir)
	{
		// Player ��ġ ����
		mPlayerPosition = playerPos;

		// Player ���� ���ϱ�
		mPlayerDir = playerDir;
		bool isLeft = false;
		if (0 >= playerDir.x)
			isLeft = true;

		// Weapon Rotate ���ϱ�
		float angle = atan2(mPlayerDir.y, mPlayerDir.x);
		mEffectAngle = angle;

		// Weapon Position ���ϱ�
		math::Vector2 armUp(-0.050f, 0.0f);
		math::Vector2 armDown(0.050f, -0.30f);
		math::Vector3 weaponPosistion = playerPos;

		// �����Ҷ����� ���� ȸ������ ��ġ�� ����
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
		// ����� ���� Weapon Transform�� �����ϱ�
		mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mWeaponTransform->SetPosition(weaponPosistion);

	}
#pragma endregion
#pragma region Inventory Call
	void WeaponScript::ChangeWeapon()
	{
		// ���� ���� �ʱ�ȭ ���ֱ�
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
				// ���� ���� ����
				mActiveArmour->Weapon.AttackDelayTime = 0.40f;
				mWeaponTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
				mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			}

			if (mWeaponTexture)
				mWeaponRenderer->ChangeSlotTexture(mWeaponTexture);
		}
	}
#pragma endregion
#pragma region Attack Process
	void WeaponScript::ModifyData()
	{
		if (enums::eWeaponName::LongSword == mWeaponType)
		{
			// Effect & Collider ����
			mProjectileSize = math::Vector2(2.50f, 1.750f);
			mProjectileCenterPadding = 0.750f;
			mProjectileValidTime = 0.30f;
			mProjectileType = enums::eProjectileType::Melee;
		}
	}
	void WeaponScript::ModifyProjectile(math::Vector2 size, float dirPadding, float validTime, enums::eProjectileType projectileType)
	{
		mProjectileSize = size;
		mProjectileCenterPadding = dirPadding;
		mProjectileValidTime = validTime;
		mProjectileType = projectileType;
	}
	void WeaponScript::DoAttack()
	{
		if (mAttackReady)
		{
			// ���� ���� �����ϱ�
			ModifyData();

			// ���� Ȱ��ȭ (Tr ����, ����Ʈ, �浹 ��ü ����)
			activeAttack();
			
			// ���� �ؽ��� & �ִϸ��̼� �����ϱ�			
			playWeaponImage();
			
			mAttackReady = false;
		}
	}
	void WeaponScript::ActiveEffect()
	{
		// ���� ���ϱ�
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		EffectWeaponScript* effect = callEffect();
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		effect->SetEffectPosition(mPlayerPosition + (playerDir * mProjectileCenterPadding));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mWeaponType);
	}
	void WeaponScript::ActiveProjectile()
	{
		mProjectileCollision = true;

		ProjectileScript* projectile = callProjectile();
		projectile->SetProjectileRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		projectile->SetProjectilePosition(mPlayerPosition);
		projectile->SetProjectileCenter(mPlayerDir * mProjectileCenterPadding);
		projectile->SetProjectileSize(mProjectileSize);
		projectile->SetProjectileType(mProjectileType);
		projectile->GetOwner()->SetObjectState(GameObject::eObjectState::Active);

	}
	void WeaponScript::activeAttack()
	{
		// ����Ʈ �����ϱ�
		ActiveEffect();
		// ����ü �����ϱ�
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