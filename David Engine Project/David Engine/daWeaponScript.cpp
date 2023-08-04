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
		// Tr세팅
		mWeaponTransform = GetOwner()->GetTransform();
		mWeaponTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// 텍스쳐 (근거리에만 쓰임) 세팅
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		ChangeWeapon(enums::eWeaponType::LongSword);
		// 애니메이터 (원거리에만 쓰임) 세팅
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
	
	// 이건 안쓸예정
	void WeaponScript::attackConditionCheck()
	{		
		if (!mAttackReady)
		{
			mAttackAccumulateTime += (float)Time::DeltaTime();

			if (mAttackDelayTime <= mAttackAccumulateTime)
			{			
				mAttackReady = true;
				// 충돌체 설정 해주기
				mAttackAccumulateTime = 0.0f;
			}
		}
	}
#pragma region 외부 호출
	void WeaponScript::DoAttack()
	{
		if (mAttackReady)
		{
			// 이펙트 적용하기
			callEffect()->PlayEffect(mWeaponType);
			// 투사체 적용하기
			//callProjectile()->PlayProjectile();
			// 무기 텍스쳐 & 애니메이션 적용하기			
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
				// 이미지 세팅
				setTextures(Resources::Find<Texture>(L"GreatSword1Texture"), Resources::Find<Texture>(L"GreatSword0Texture"));
				mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
				// 이펙트 세팅
				// 충돌체 세팅
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

		// Position 바꾸는 함수

		// Rotate 바꾸는 함수

		// 활성화 함수


		effect->SetEffectPosition(mWeaponTransform->GetPosition() + (mWeaponTransform->Up() * 0.60f));
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		
	}
#pragma endregion
}