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

		, mChaseDurationTime(0.0f)
		, mChaseDurationDecay(0.0f)

		, mPrepareAttack(false)
		, mPrepareDurationTime(0.0f)
		, mPrepareDurationDecay(0.0f)
	{
	}

	SkelScript::~SkelScript()
	{
	}

#pragma region Basic Func
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

		// ���� �ʱ�ȭ
		mCreatureStat.MoveSpeed = 1.750f;		

		mChaseDurationTime = 0.50f;
		mPrepareDurationTime = 1.750f;
	}

	void SkelScript::Update()
	{
		skelFSM();
		visualUpdate();
		lifeCheck();
	}
#pragma endregion


#pragma region Common Func
	void SkelScript::calcCreatureDir()
	{
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		mDistanceFromPlayer = mPlayerScript->GetOwner()->GetTransform()->GetPosition().x - mCreatureTransform->GetPosition().x;
		if (0 >= mDistanceFromPlayer)
			mCreatureDir = -math::Vector2::UnitX;
		else
			mCreatureDir = math::Vector2::UnitX;
	}
	void SkelScript::visualUpdate()
	{
		mCreatureWeaponScript->SetWeaponTransform(mCreatureTransform->GetPosition());
		ReverseTexture();
	}
	void SkelScript::lifeCheck()
	{
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eCreatureState::Dead);
	}
#pragma endregion



	void SkelScript::chasingTimeout()
	{
		if (eCreatureState::Chase != mCreatureActiveState)
			return;

		if (eCreatureState::Chase == mCreatureActiveState
			&& mDetectPlayer)
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
	void SkelScript::skelFSM()
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
		// Chace ���·� ��ȯ
		if (mDetectPlayer)
			ChangeState(eCreatureState::Chase);
	}

	void SkelScript::SkelHandleChase()
	{
		// �̵�
		trackingPlayer();
		// �߰� ���ѽð� & ���� �޼��� ���� ���·� ����
		findingPlayer();
		
	}
	void SkelScript::trackingPlayer()
	{
		// ���� ���ϱ�		
		calcCreatureDir();

		// �̵��ϱ� *�̵��ϱ����� �� �浹 ���� Ȯ���ϴ� �ڵ� �ʿ���
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();
		skelPosition.x += mCreatureDir.x * mCreatureStat.MoveSpeed * (float)Time::DeltaTime();
		mCreatureTransform->SetPosition(skelPosition);				
	}
	void SkelScript::findingPlayer()
	{
		// �÷��̾ �������� ���� �ִٸ� or ���ٸ�
		if (mDetectPlayer)
		{
			mChaseDurationDecay = mChaseDurationTime;
		}
		else
		{
			mChaseDurationDecay -= (float)Time::DeltaTime();
			if (0.0f >= mChaseDurationDecay)
			{
				mChaseDurationDecay = mChaseDurationTime;

				ChangeState(eCreatureState::Idle);
			}
		}
		// ���� ������ �����ߴٸ�
		if (abs(mDistanceFromPlayer) <= mCreatureStat.AttackRange)
		{
			mChaseDurationDecay = mChaseDurationTime;
			ChangeState(eCreatureState::Attack);
		}
	}

	void SkelScript::SkelHandleAttack()
	{
		// ���� �غ� ������
		prepareForAttack();
		// ���� ����
		doAttack();
		
		// �ĵ�����
		readyForAttackDelay();

	}

	void SkelScript::prepareForAttack()
	{
		// ������ �����ٸ� ����
		if (mPrepareAttack)
			return;

		// ���� ����
		mPrepareDurationDecay -= (float)Time::DeltaTime();
		if (0 >= mPrepareDurationDecay)
		{
			mPrepareDurationDecay = mPrepareDurationTime;	// ���� �ð��� �ʱ�ȭ
			mPrepareAttack = true;							// ����, �ĵ� ���� Ȱ��ȭ
		}
	}
	void SkelScript::doAttack()
	{
		if (!mPrepareAttack)
			return;
		else
		{
			if (!mAttackProgress)
			{
				mCreatureAnimator->PlayAnimation(L"SkelIdle");
				mAttackProgress = true;								// �ִϸ��̼� ȣ�� ����
			}
				mCreatureWeaponScript->DoAttack();
		}
	}
	void SkelScript::readyForAttackDelay()
	{
		if (!mPrepareAttack)
			return;
		else
		{
			mReadyDurationDecay -= (float)Time::DeltaTime();
			if (0 >= mReadyDurationDecay)
			{
				mReadyDurationDecay = mReadyDurationTime;	// �ĵ� �ð��� �ʱ�ȭ
				mPrepareAttack = false;						// ���� ���� �ʱ�ȭ
				mAttackProgress = false;					// ���� �ִϸ��̼� ȣ�� ���� �ʱ�ȭ
				ChangeState(eCreatureState::Chase);			// ���� ����
			}
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
			mCreatureAnimator->PlayAnimation(L"SkelIdle");
			break;
		case da::eCreatureState::Dead:
			GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
			break;
		default:
			break;
		}
	}
#pragma endregion
	
}