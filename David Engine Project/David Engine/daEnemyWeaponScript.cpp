#include "daEnemyWeaponScript.h"
#include "daGameDataManager.h"
#include "daTime.h"
#include "daInput.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daAnimator.h"
#include "daMeshRenderer.h"

namespace da
{
	EnemyWeaponScript::EnemyWeaponScript()
		: mEnemyWeaponTransform(nullptr)
		, mEnemyWeaponRenderer(nullptr)
		, mEnemyWeaponAnimator(nullptr)
		, mEnemyWeaponCollider(nullptr)
		, mEnemyWeaponTexture(nullptr)
		, mPlayerDamaged(false)
	{
	}
	EnemyWeaponScript::~EnemyWeaponScript()
	{
	}
	void EnemyWeaponScript::Initialize()
	{
		mEnemyWeaponTransform = GetOwner()->GetComponent<Transform>();
		mEnemyWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mEnemyWeaponAnimator = GetOwner()->AddComponent<Animator>();
		mEnemyWeaponCollider = GetOwner()->AddComponent<Collider2D>();
		
		mEnemyWeaponTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);

		// 애니메이션 & 이벤트
		
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordIdle", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 1, math::Vector2::Zero, 0.0f, 70.0f);
		
		
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordSwing", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 70.0f);
		mEnemyWeaponAnimator->ActionEvent(L"EnemyGreatSwordSwing", 7) = std::bind(&EnemyWeaponScript::activeAttack, this);
		mEnemyWeaponAnimator->CompleteEvent(L"EnemyGreatSwordSwing") = std::bind(&EnemyWeaponScript::inactiveAttack, this);
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordIdle", false);
	}
	void EnemyWeaponScript::Update()
	{
	}
	void EnemyWeaponScript::DoAttack()
	{
		// 애니메이션
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordSwing", false);
		// 충돌체 활성화	*액션이벤트에서 진행해야함
		// mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}

	// 특정 프레임에 호출됨
	void EnemyWeaponScript::activeAttack()
	{
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}

	void EnemyWeaponScript::inactiveAttack()
	{
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);
	}

	void EnemyWeaponScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			GameDataManager::GetDamage(4.0f);
			mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);				
		}		
	}
}
