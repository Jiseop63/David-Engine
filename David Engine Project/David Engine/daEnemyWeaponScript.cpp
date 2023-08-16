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
		, mCreatureAttackReady(false)
		, mCreatureAttackAccumulateTime(0.0f)
		, mCreatureAttackDelayTime(0.0f)
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

		mCreatureAttackDelayTime = 3.20f;

		

		mEnemyWeaponAnimator->Create(L"EnemyGreatSword", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 45.0f);
	}
	void EnemyWeaponScript::Update()
	{
		// 공격 딜레이 설정

		if (!mCreatureAttackReady)
		{
			mCreatureAttackAccumulateTime += (float)Time::DeltaTime();

			if (mCreatureAttackDelayTime <= mCreatureAttackAccumulateTime)
			{
				mCreatureAttackReady = true;
				// 충돌체 설정 해주기
				mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Inactive);
				mCreatureAttackAccumulateTime = 0.0f;
			}
		}
	}
	void EnemyWeaponScript::SetWeaponTransform(math::Vector3 ownerPos, math::Vector2 ownerDir)
	{		
		mEnemyWeaponTransform->SetPosition(ownerPos);

		bool isLeft = false;
		if (0 >= ownerDir.x)
			isLeft = true;
		mEnemyWeaponRenderer->SetReverse(isLeft);
	}
	void EnemyWeaponScript::DoAttack()
	{
		if (mCreatureAttackReady)
		{
			// 공격 활성화 (Tr 세팅, 이펙트, 충돌 객체 세팅)
			activeAttack();
			// 무기 텍스쳐 & 애니메이션 적용하기

			mCreatureAttackReady = false;
		}
	}
	void EnemyWeaponScript::activeAttack()
	{
		mEnemyWeaponAnimator->PlayAnimation(L"EnemyGreatSword", false);
		//충돌체 활성화
		mEnemyWeaponCollider->SetDetectionType(Collider2D::eDetectionType::Default);
	}
	void EnemyWeaponScript::OnCollisionEnter(Collider2D* other)
	{

		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{			
			// 피격 호출
			GameDataManager::GetDamage(4.0f);
		}		
	}
}
