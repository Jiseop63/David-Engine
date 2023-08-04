#include "daWeaponScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameDataManager.h"

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
		, mPlayerDir(math::Vector3::Zero)
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

	}
	void WeaponScript::Update()
	{
		attackConditionCheck();
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
	
	// �̰� �Ⱦ�����
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
#pragma region �ܺ� ȣ��
	void WeaponScript::DoAttack()
	{
		if (mAttackReady)
		{
			// ����Ʈ �����ϱ�
			callEffect()->PlayEffect(mWeaponType);
			// ����ü �����ϱ�
			//callProjectile()->PlayProjectile();
			// ���� �ؽ��� & �ִϸ��̼� �����ϱ�			
			playWeaponImage();
			
			mAttackReady = false;
		}		
	}
	void WeaponScript::ChangeWeapon(enums::eWeaponType weaponType)
	{
		mWeaponType = weaponType;
		switch (weaponType)
		{
		case da::enums::eWeaponType::Default:
			break;
		case da::enums::eWeaponType::LongSword:
			WeaponInit();
			break;
		default:
			break;
		}
	}
	void WeaponScript::WeaponInit(bool isMelee)
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
	void WeaponScript::AddEffectObject(GameObject* object)
	{
		mEffects.push_back(object->AddComponent<EffectWeaponScript>());
	}
	void WeaponScript::AddProjectileObject(GameObject* object)
	{
		mProjectiles.push_back(object->AddComponent<ProjectileScript>());
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
	void WeaponScript::ActiveEffect(EffectWeaponScript* effect, const std::wstring name)
	{
		if (!effect)
			return;

		// Position �ٲٴ� �Լ�

		// Rotate �ٲٴ� �Լ�

		// Ȱ��ȭ �Լ�


		effect->SetEffectPosition(mWeaponTransform->GetPosition() + (mWeaponTransform->Up() * 0.60f));
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		
	}
#pragma endregion
}