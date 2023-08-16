#include "daSkelScript.h"

#include "daTime.h"

#include "daResources.h"
#include "daGameObject.h"
#include "daProjectileScript.h"
#include "daWeaponScript.h"
#include "daEffectWeaponScript.h"

#include "daEnemyWeaponScript.h"
#include "daEffectEnemyScript.h"
#include "daPlayerScript.h"

namespace da
{
	SkelScript::SkelScript()
		: mGotoReturn(false)
		, mReturnAccumulateTime(0.0f)
		, mReturnDelayTime(0.0f)
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
			mCreatureFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		}

		// 애니메이션 설정하기
		std::shared_ptr<Texture> weapon = Resources::Find<Texture>(L"EnemyGreatSword");
		mCreatureAnimator->Create(L"SkelSwing", weapon, math::Vector2(0.0f, 0.0f), math::Vector2(51.0f, 49.0f), 16, math::Vector2(0.0f, 0.0f), 0.1f);

		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"SkelSprite");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");

		mReturnDelayTime = 1.50f;
	}

	void SkelScript::Update()
	{
		if (eCreatureState::Chase == mCreatureActiveState
			&& mPlayerFind)
		{
			mReturnAccumulateTime = 0.0f;			
		}
		else
		{
			mReturnAccumulateTime += (float)Time::DeltaTime();
			if (mReturnDelayTime <= mReturnAccumulateTime)
			{
				mReturnAccumulateTime = 0.0f;
				mGotoReturn = true;
			}
		}
		
		ReverseTexture();


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
		mGotoReturn = false;

		if (mPlayerFind)
			ChangeState(eCreatureState::Chase);		
	}

	void SkelScript::SkelHandleChase()
	{
		float moveSpeed = 1.0f;
		math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();
		
		float distanceX = playerPosition.x - skelPosition.x;
		if (!mGotoReturn)
		{
			if (0 >= distanceX)
				mCreatureDir = -math::Vector2::UnitX;
			else
				mCreatureDir = math::Vector2::UnitX;

			skelPosition.x += mCreatureDir.x * moveSpeed * (float)Time::DeltaTime();
			mCreatureTransform->SetPosition(skelPosition);
		}
		else
		{
			float homeDir = mStandingPosition.x - skelPosition.x;
			if (0 >= homeDir)
				mCreatureDir = -math::Vector2::UnitX;
			else
				mCreatureDir = math::Vector2::UnitX;

			skelPosition.x += mCreatureDir.x * moveSpeed * (float)Time::DeltaTime();

			mCreatureTransform->SetPosition(skelPosition);

			if (0.5f >= abs(skelPosition.x - mStandingPosition.x))
			{
				ChangeState(eCreatureState::Idle);
			}
		}

		/*if (abs(distanceX) <= mCreatureStat.AttackRange)
		{
			mCreatureWeaponScript->IsCreatureAttackReady();
			ChangeState(eCreatureState::Attack);
		}*/

		

	}

	void SkelScript::SkelHandleAttack()
	{
		// DoAttack!
		mCreatureWeaponScript->SetWeaponTransform(mCreatureTransform->GetPosition(), mCreatureDir);
		mCreatureWeaponScript->DoAttack();
		
		// 끝나는 조건을 확인해야함
	}

	void SkelScript::SkelHandleDead()
	{
		if (!mIsDead)
		{
			// 사망 이펙트 실행
			mCreatureWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
			mEnemyEffectScript->SetEffectPosition(mCreatureTransform->GetPosition() + math::Vector3(0.0f, -0.20f, 0.0f));
			mEnemyEffectScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			mEnemyEffectScript->PlayEffect(L"Dying");
		}
		mIsDead = true;
	}
}