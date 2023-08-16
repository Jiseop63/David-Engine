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
		, mInAttack(false)
		, mPlayerDamaged(false)
		, mReadyToAttack(false)
		, mCooldownAccumulateTime(0.0f)
		, mAttackCooldownTime(0.0f)
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

		mEnemyWeaponAnimator->StartEvent(L"EnemyGreatSwordSwing") = std::bind(&EnemyWeaponScript::AttackStart, this);
		mEnemyWeaponAnimator->CompleteEvent(L"EnemyGreatSwordSwing") = std::bind(&EnemyWeaponScript::AttackFinished, this);

		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordIdle", false);

		// 조건 변수
		mAttackCooldownTime = 4.0f;

	}
	void EnemyWeaponScript::Update()
	{
		if (!mReadyToAttack)
		{
			mCooldownAccumulateTime += (float)Time::DeltaTime();
			if (mAttackCooldownTime <= mCooldownAccumulateTime)
			{
				mReadyToAttack = true;
				mCooldownAccumulateTime = 0.0f;
			}
		}
		if (Input::GetKeyDown(eKeyCode::B))
		{
			mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordSwing", false);
		}
	}
	void EnemyWeaponScript::DoAttack()
	{
		if (mReadyToAttack)
			activeAttack();
		
	}
	void EnemyWeaponScript::activeAttack()
	{
		mReadyToAttack = false;
		// 애니메이션
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordSwing", false);
		//충돌체 활성화
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void EnemyWeaponScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			if (!mPlayerDamaged)
			{
				GameDataManager::GetDamage(4.0f);
				mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);
				mPlayerDamaged = true;
			}
				
		}		
	}
}
