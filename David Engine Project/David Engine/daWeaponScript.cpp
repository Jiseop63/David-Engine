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

		, mInventoryData{}
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)

		, mWeaponType(enums::eWeaponType::LongSword)
		, mPlayerDir(math::Vector2::Zero)
		, mAttackReady(true)
		, mWeaponAttacked(false)
		, mEffectAngle(0.0f)

		, mAttackAccumulateTime(0.0f)
		, mAttackDelayTime(0.40f)
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
		ChangeWeapon(enums::eWeaponType::LongSword);
		// �ִϸ����� (���Ÿ����� ����) ����
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();

		mInventoryData = &GameDataManager::GetInventory();
		weaponInit();
	}
	void WeaponScript::Update()
	{
		attackConditionCheck();
		calcWeaponAngle();
	}
		
#pragma region common func
	void WeaponScript::attackConditionCheck()
	{		
		if (!mAttackReady)
		{
			mAttackAccumulateTime += (float)Time::DeltaTime();

			if (mAttackDelayTime <= mAttackAccumulateTime)
			{			
				mAttackReady = true;
				// �浹ü ���� ���ֱ�
				mAttackAccumulateTime = 0.0f;
			}
		}
	}
	void WeaponScript::calcWeaponAngle()
	{
		// ���� ȸ���� ����
		float angle = atan2(mPlayerDir.y, mPlayerDir.x);
		mEffectAngle = angle;

		// ���� ����� ������ ���� ȸ���� ����
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
#pragma endregion

#pragma region weapon func
	void WeaponScript::ChangeWeapon(enums::eWeaponType weaponType)
	{
		mWeaponType = weaponType;
		switch (weaponType)
		{
		case da::enums::eWeaponType::Default:
			break;
		case da::enums::eWeaponType::LongSword:
			weaponInit();
			break;
		default:
			break;
		}
	}
	void WeaponScript::DoAttack()
	{
		if (mAttackReady)
		{
			// ���� Ȱ��ȭ (Tr ����, ����Ʈ, �浹 ��ü ����)
			activeAttack();
			
			// ���� �ؽ��� & �ִϸ��̼� �����ϱ�			
			playWeaponImage();
			
			mAttackReady = false;
		}		
	}
	void WeaponScript::weaponInit(bool isMelee)
	{
		if (isMelee)
		{
			if (enums::eWeaponType::LongSword == mWeaponType)
			{
				// �̹��� ����
				setTextures(Resources::Find<Texture>(L"GreatSword1Texture"), Resources::Find<Texture>(L"GreatSword0Texture"));
				mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
				// ����Ʈ ����
				// �浹ü ����
			}
		}
	}
	void WeaponScript::activeAttack()
	{
		// ���� ���ϱ�
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		// ����Ʈ �����ϱ�
		EffectWeaponScript* effect = callEffect();
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));		
		effect->SetEffectPosition(mWeaponTransform->GetPosition() + (playerDir * 0.60f));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mWeaponType);

		// ����ü �����ϱ�
		ProjectileScript* projectile = callProjectile();
		projectile->SetProjectileRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		projectile->SetProjectilePosition(mWeaponTransform->GetPosition());
		projectile->SetProjectileCollider(math::Vector2(2.50f, 1.50f), mPlayerDir * 0.60f);
		projectile->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void WeaponScript::playWeaponImage()
	{
		// Change Weapon Texture
		{
			if (mWeaponAttacked)
			{
				mWeaponAttacked = false;
				mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
			}
			else
			{
				mWeaponAttacked = true;
				mWeaponRenderer->ChangeSlotTexture(mSecondTexture);
			}
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