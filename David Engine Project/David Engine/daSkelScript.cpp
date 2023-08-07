#include "daSkelScript.h"
#include "daResources.h"
#include "daGameObject.h"

namespace da
{
	SkelScript::SkelScript()
	{
	}

	SkelScript::~SkelScript()
	{
	}

	void SkelScript::Initialize()
	{
		// 컴포넌트 가져오기
		CreatureScript::Initialize();
		
		// 충돌체 설정하기
		{
			// body
			mCreatureBodyCollider->SetSize(math::Vector2(0.30f, 0.40f));
			mCreatureBodyCollider->SetCenter(math::Vector2(0.0f, -0.10f));
		}
		{
			// foot
			mCreatureFootCollider->SetSize(math::Vector2(0.050f, 0.050f));
			mCreatureFootCollider->SetCenter(math::Vector2(0.0f, -0.450f));
			mCreatureFootCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
		}

		// 애니메이션 설정하기
		std::shared_ptr<Texture> weapon = Resources::Load<Texture>(L"EnemyGreatSword", L"..\\Resources\\Texture\\Monster\\Skel\\GreatSwordSpriteSheet.png");
		mCreatureAnimator->Create(L"SkelSwing", weapon, math::Vector2(0.0f, 0.0f), math::Vector2(51.0f, 49.0f), 16, math::Vector2(0.0f, 0.0f), 0.1f);

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");
	}

	void SkelScript::Update()
	{
		SkelFSM();
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eCreatureState::Dead);
	}

	void SkelScript::ChangeState(eCreatureState state)
	{
		mCreatureActiveState = state;
		switch (mCreatureActiveState)
		{
		case da::eCreatureState::Idle:
			mCreatureAnimator->PlayAnimation(L"SkelIdle");
			break;
		case da::eCreatureState::Chase:
			mCreatureAnimator->PlayAnimation(L"SkelMove");
			break;
		case da::eCreatureState::Attack:
			mCreatureAnimator->PlayAnimation(L"SkelAttact");
			break;
		case da::eCreatureState::Dead:
			GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
			break;
		default:
			break;
		}
	}

	void SkelScript::SkelFSM()
	{
		switch (mCreatureActiveState)
		{
		case da::eCreatureState::Idle:
			SkelHandleIdle();
			break;
		case da::eCreatureState::Chase:
			SkelHandleChase();
			break;
		case da::eCreatureState::Attack:
			SkelHandleAttack();
			break;
		case da::eCreatureState::Dead:
			SkelHandleDead();
			break;
		default:
			break;
		}
	}

	void SkelScript::SkelHandleIdle()
	{
		// 탐지에 필요한 변수들
		//float detectYRange = 2.50f;
		//mCreatureStat.DetectRange;
		//
		//// 위치 구하기
		//math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		//math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		//// 플레이어를 탐지
		//float distanceX = skelPosition.x - playerPosition.x;
		//float distanceY = skelPosition.y - playerPosition.y;

		//if (abs(distanceX) >= mCreatureStat.DetectRange
		//	&& abs(distanceY) >= detectYRange)
		//{
		//	ChangeState(eCreatureState::Chase);
		//}
	}

	void SkelScript::SkelHandleChase()
	{
		math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		float distance = math::Vector3::Distance(skelPosition, playerPosition);
		// 플레이어를 탐지
		float distanceX = skelPosition.x - playerPosition.x;
		float distanceY = skelPosition.y - playerPosition.y;

		// 공격 범위까지 접근
		if (distance >= mCreatureStat.AttackRange)
		{
			ChangeState(eCreatureState::Attack);
		}

		// 되돌아와서 Idle
	}

	void SkelScript::SkelHandleAttack()
	{
		// DOAttack!
		
		// retChase
	}

	void SkelScript::SkelHandleDead()
	{
		if (!mIsDead)
		{
			// 사망 이펙트 실행
			mEnemyEffectScript->PlayEffect(L"Dying");
			mCreatureWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
		}
		mIsDead = true;
	}

	void SkelScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::PlayableAttackCollider == other->GetOwner()->GetLayerType())
		{
			// 피격 호출
			OnDamaged();
		}

	}

}