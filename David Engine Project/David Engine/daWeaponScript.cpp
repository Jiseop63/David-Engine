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
		, mEffects{}

		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
		, mWeaponAttacked(false)
		, mEffectAngle(0.0f)

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
		// Tr세팅
		mWeaponTransform = GetOwner()->GetTransform();
		mWeaponTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// 텍스쳐 (근거리에만 쓰임) 세팅
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		ChangeWeapon(enums::eWeaponType::LongSword);

		// 충돌체 세팅
		mWeaponCollider = GetOwner()->AddComponent<Collider2D>();
		mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);

		// 애니메이터 (원거리에만 쓰임) 세팅
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
#pragma region 외부 호출
	void WeaponScript::DoAttack()
	{
		if (Collider2D::eColliderDetection::Inactive == mWeaponCollider->GetColliderDetection())
		{
			// 이펙트 적용하기
			ActiveEffect(callEffect(), L"GreatSwing");

			// 무기 이미지 & 애니메이션 적용하기
			if (mWeaponAttacked)
				mWeaponAttacked = false;
			else
				mWeaponAttacked = true;
			ChangeWeaponTexture();

			// 추후에 사라질 예정
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
	void WeaponScript::AddEffectObject(GameObject* object)
	{
		mEffects.push_back(object->AddComponent<EffectWeaponScript>());
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
	void WeaponScript::ActiveEffect(EffectWeaponScript* effect, const std::wstring name)
	{
		if (!effect)
			return;

		effect->SetEffectPosition(mWeaponTransform->GetPosition());
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(name);
	}
#pragma endregion
}