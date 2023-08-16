#include "daEnemyWeaponScript.h"
#include "daGameDataManager.h"
#include "daTime.h"

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
		

		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);

		// 애니메이션 & 이벤트
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordIdle", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 1, math::Vector2::Zero, 0.0f, 45.0f);
		mEnemyWeaponAnimator->Create(L"EnemyGreatSwordSwing", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 45.0f);

		mEnemyWeaponAnimator->StartEvent(L"EnemyGreatSwordSwing") = std::bind(&EnemyWeaponScript::AttackStart, this);
		mEnemyWeaponAnimator->CompleteEvent(L"EnemyGreatSwordSwing") = std::bind(&EnemyWeaponScript::AttackFinished, this);

			
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSwordIdle", false);
	}
	void EnemyWeaponScript::Update()
	{
	}
	void EnemyWeaponScript::DoAttack()
	{
		activeAttack();
	}
	void EnemyWeaponScript::activeAttack()
	{
		// 애니메이션
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSword", false);
		//충돌체 활성화
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void EnemyWeaponScript::OnCollisionEnter(Collider2D* other)
	{

		//if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
		//	&& other->IsBody())
		//{			
		//	// 피격 호출
		//	GameDataManager::GetDamage(4.0f);
		//}
	}
}
