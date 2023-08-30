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
		// ������Ʈ ��������
		CreatureScript::Initialize();
		
		// �浹ü �����ϱ�
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

		// �ִϸ��̼� �����ϱ�
		
		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"SkelSprite");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");

		// ���� ������ �ʱ�ȭ
		mCreatureStat.MoveSpeed = 1.750f;
		mReturnDelayTime = 1.50f;
		
	}

	void SkelScript::Update()
	{
		chasingTimeout();
		attackCooldownReady();
		mCreatureWeaponScript->SetWeaponTransform(mCreatureTransform->GetPosition());
		ReverseTexture();
		SkelFSM();

		if (0 >= mCreatureStat.CurHP)
			ChangeState(eCreatureState::Dead);
	}

	void SkelScript::chasingTimeout()
	{
		if (eCreatureState::Chase != mCreatureActiveState)
			return;

		if (eCreatureState::Chase == mCreatureActiveState
			&& mPlayerFind)
			mReturnAccumulateTime = 0.0f;
		else
		{
			mReturnAccumulateTime += (float)Time::DeltaTime();
			if (mReturnDelayTime <= mReturnAccumulateTime)
			{
				mReturnAccumulateTime = 0.0f;
				mGotoReturn = true;
			}
		}
	}
	void SkelScript::attackCooldownReady()
	{
		
	}
	
#pragma region FSM Func
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
		
		if (!mGotoReturn)
			moveToAttackRange();
		else
			returnIdle();		
	}

	void SkelScript::SkelHandleAttack()
	{
		// �������� �ʾҴµ�, �Ÿ��� �־����� chase
		if (!mCreatureWeaponScript->IsAttacking())
		{
			float distanceX = calcCreatureDir(mPlayerScript->GetOwner()->GetTransform()->GetPosition(), mCreatureTransform->GetPosition());
			if (abs(distanceX) >= mCreatureStat.AttackRange)
				ChangeState(eCreatureState::Chase);



			if (mCreatureWeaponScript->IsReadyToAttack())
				mCreatureWeaponScript->DoAttack();
			// doAttack�� ������ �������� ���� �� ������ ����� ��. ��, ������ ������ �÷��̾ �Ѿƿ����� �Ұ���
		}
				
	}
	void SkelScript::SkelHandleDead()
	{
		if (!mIsDead)
		{
			// ��� ����Ʈ ����
			mCreatureWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
			mEnemyEffectScript->SetEffectPosition(mCreatureTransform->GetPosition() + math::Vector3(0.0f, -0.20f, 0.0f));
			mEnemyEffectScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			mEnemyEffectScript->PlayEffect(L"Dying");
		}
		mIsDead = true;
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
#pragma endregion
	float SkelScript::calcCreatureDir(math::Vector3 targetPosition, math::Vector3 myPosition)
	{

		float retDistance = targetPosition.x - myPosition.x;
		if (0 >= retDistance)
			mCreatureDir = -math::Vector2::UnitX;
		else
			mCreatureDir = math::Vector2::UnitX;
		return retDistance;
	}
	void SkelScript::moveToAttackRange()
	{
		// ���� ���ϱ�
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();
		float distanceX = calcCreatureDir(mPlayerScript->GetOwner()->GetTransform()->GetPosition(), skelPosition);

		// ���� ����
		if (abs(distanceX) <= mCreatureStat.AttackRange)
			ChangeState(eCreatureState::Attack);

		// �̵�
		// �̵��� �����ֱ�
		skelPosition.x += mCreatureDir.x * mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		mCreatureTransform->SetPosition(skelPosition);
	}
	void SkelScript::returnIdle()
	{
		// ���� ���ϱ�
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();
		float homeDir = calcCreatureDir(mStandingPosition, skelPosition);

		// ���� ����
		if (0.5f >= abs(skelPosition.x - mStandingPosition.x))
			ChangeState(eCreatureState::Idle);
		
		// �̵�
		skelPosition.x += mCreatureDir.x * mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		mCreatureTransform->SetPosition(skelPosition);
	}	
}