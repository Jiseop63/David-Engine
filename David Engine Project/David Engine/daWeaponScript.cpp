#include "daWeaponScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"

namespace da
{
	WeaponScript::WeaponScript()
		: mWeaponTransform(nullptr)
		, mWeaponRenderer(nullptr)
		, mWeaponCollider(nullptr)
		, mWeaponAnimator(nullptr)

		, mEffectScript(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mWeaponAttacked(false)
		, mWeaponType(enums::eWeaponType::LongSword)
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

		// �浹ü ����
		mWeaponCollider = GetOwner()->AddComponent<Collider2D>();
		mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);

		// �ִϸ����� (���Ÿ����� ����) ����
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();
	}
	void WeaponScript::Update()
	{
		AttackLogic();
	}
	
	void WeaponScript::ChangeWeaponTexture()
	{
		if (mWeaponAttacked)
			mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
		else
			mWeaponRenderer->ChangeSlotTexture(mSecondTexture);
	}
	void WeaponScript::AttackLogic()
	{
		if (Collider2D::eColliderDetection::Default == mWeaponCollider->GetColliderDetection())
		{
			mAttackAccumulateTime += (float)Time::DeltaTime();

			if (mAttackDelayTime <= mAttackAccumulateTime)
			{
				mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);
				mAttackAccumulateTime = 0.0f;
			}
		}
	}
#pragma region �ܺ� ȣ��
	void WeaponScript::DoAttack()
	{
		if (Collider2D::eColliderDetection::Inactive == mWeaponCollider->GetColliderDetection())
		{
			mEffectScript->PlayEffect();
			// ��� �ٲ��ֱ�
			if (mWeaponAttacked)
				mWeaponAttacked = false;
			else
				mWeaponAttacked = true;
			ChangeWeaponTexture();
			mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Default);
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
				SetTextures(Resources::Find<Texture>(L"GreatSword1Texture"), Resources::Find<Texture>(L"GreatSword0Texture"));
				mWeaponRenderer->ChangeSlotTexture(mFirstTexture);
			}
		}
	}
	void WeaponScript::SetEffectObject(GameObject* object)
	{
		mEffectScript = object->AddComponent<EffectScript>();
	}
#pragma endregion
}