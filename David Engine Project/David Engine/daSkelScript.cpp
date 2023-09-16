#include "daSkelScript.h"

#include "daSceneManager.h"


#include "daTime.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daSkelCombatScript.h"
#include "daEffectEnemyScript.h"
#include "daPlayerScript.h"

namespace da
{
	SkelScript::SkelScript()
		: mChaseDurationTime(0.0f)
		, mChaseDurationDecay(0.0f)
		, mDistanceFromPlayer(0.0f)

		, mPrepareAttack(false)
		, mPrepareDurationTime(0.0f)
		, mPrepareDurationDecay(0.0f)

		, mAttackProgress(false)
		, mReadyDurationTime(0.0f)
		, mReadyDurationDecay(0.0f)
	{
	}

	SkelScript::~SkelScript()
	{
	}

#pragma region Basic Func
	void SkelScript::Initialize()
	{
		// ������Ʈ ��������
		MonsterScript::Initialize();
		
		// �浹ü �����ϱ�
		// body & foot
		{
			mCreatureBodyCollider->SetSize(math::Vector2(0.30f, 0.40f));
			mCreatureBodyCollider->SetCenter(math::Vector2(0.0f, -0.10f));
			mCreatureFootCollider->SetSize(math::Vector2(0.050f, 0.050f));
			mCreatureFootCollider->SetCenter(math::Vector2(0.0f, -0.450f));
		}
		// senser
		{
			mMonsterSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
			mMonsterSensorCollider->SetSize(math::Vector2(4.50f, 1.90f));
			mMonsterSensorCollider->SetCenter(math::Vector2(0.0f, 0.450f));
		}

		// �ִϸ��̼� �����ϱ�

		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"SkelSprite");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");

		mPlayerScript = SceneManager::GetPlayerScript();


		// ���� �ʱ�ȭ
		mCreatureStat.MaxHP = 25.0f;
		mCreatureStat.CurHP = 25.0f;

		mCreatureStat.MoveSpeed = 1.250f;
		mMonsterAttackStat.DetectRange = 2.50f;

		mMonsterAttackStat.AttackRange = 0.750f;
		mMonsterAttackStat.AttackDelayAccumulateTime = 0.0f;
		mMonsterAttackStat.AttackDelay = 4.0f;

		mCreatureStat.MoveSpeed = 1.50f;

		mChaseDurationTime = 1.50f;
		mChaseDurationDecay = mChaseDurationTime;
		mPrepareDurationTime = 0.250f;
		mPrepareDurationDecay = mPrepareDurationTime;
		mReadyDurationTime = 2.50f;
		mReadyDurationDecay = mReadyDurationTime;
	}

	void SkelScript::Update()
	{
		skelFSM();
		lifeCheck();
	}
	void SkelScript::LateUpdate()
	{
		CreatureScript::visualUpdate();
	}
#pragma endregion


#pragma region Common Func
	void SkelScript::calcTargetDir()
	{
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		mDistanceFromPlayer = mPlayerScript->GetOwner()->GetTransform()->GetPosition().x - mCreatureTransform->GetPosition().x;
		if (0 >= mDistanceFromPlayer)
			mCreatureDir = -math::Vector2::UnitX;
		else
			mCreatureDir = math::Vector2::UnitX;
	}
	void SkelScript::lifeCheck()
	{
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eMonsterState::Dead);
	}
#pragma endregion



	
#pragma region FSM Func
	void SkelScript::skelFSM()
	{
		switch (mMonsterActiveState)
		{
		case da::eMonsterState::Idle:
			SkelHandleIdle();
			break;
		case da::eMonsterState::Chase:
			SkelHandleChase();
			break;
		case da::eMonsterState::Attack:
			SkelHandleAttack();
			break;
		case da::eMonsterState::Dead:
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
			ChangeState(eMonsterState::Chase);
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
		calcTargetDir();

		// �̵��ϱ� *�̵��ϱ����� �� �浹 ���� Ȯ���ϴ� �ڵ� �ʿ���
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();	// �� �浹 üũ
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();									// �� ��ġ
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();						// �̵���
		math::Vector2 moveDir = daRotateVector2(mCreatureDir, mCreatureFootCollider->GetEnvRotate());	// ȸ������ ����� �̵�����
		math::Vector2 movePosition = moveDir * moveMagnitude;											// �̵���ġ
		skelPosition.x += movePosition.x;																// �̵��� ��ġ
		skelPosition.y += movePosition.y;																// �̵��� ��ġ

		// �� �浹 ���⿡ ���� �̵� ����
		if (0 <= mCreatureDir.x)
			moveRight(); 
		else
			moveLeft();
	}
	void SkelScript::findingPlayer()
	{
		// �÷��̾ �������� ���� �ִٸ� or ���ٸ�
		if (mDetectPlayer)
			mChaseDurationDecay = mChaseDurationTime;
		else
		{
			mChaseDurationDecay -= (float)Time::DeltaTime();
			if (0.0f >= mChaseDurationDecay)
			{
				mChaseDurationDecay = mChaseDurationTime;
				ChangeState(eMonsterState::Idle);
			}
		}
		// ���� ������ �����ߴٸ�
		if (abs(mDistanceFromPlayer) <= mMonsterAttackStat.AttackRange)
		{
			mChaseDurationDecay = mChaseDurationTime;
			ChangeState(eMonsterState::Attack);
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
		// ������ �����ٸ� �ѱ��
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
		// ���� �������̰� �������� �ѱ��
		if (!mPrepareAttack)
			return;

		// �����ϱ� ���̶��
		if (!mAttackProgress)
		{
			mCreatureAnimator->PlayAnimation(L"SkelIdle");	// �ִϸ��̼� ȣ��
			mMonsterCombatScript->ToDoAttack();				// ���� ��� ȣ��
			mAttackProgress = true;							// ���� �������� �ѱ��
		}
	}
	void SkelScript::readyForAttackDelay()
	{
		if (!mPrepareAttack)
			return;

		if (mAttackProgress)
		{
			mReadyDurationDecay -= (float)Time::DeltaTime();
			if (0 >= mReadyDurationDecay)
			{
				mReadyDurationDecay = mReadyDurationTime;	// �ĵ� �ð��� �ʱ�ȭ
				mPrepareAttack = false;						// ���� ���� �ʱ�ȭ
				mAttackProgress = false;					// ���� �ִϸ��̼� ȣ�� ���� �ʱ�ȭ
				ChangeState(eMonsterState::Chase);			// ���� ����
			}
		}
	}
	void SkelScript::SkelHandleDead()
	{
		if (!mIsDead)
		{
			// ��� ����Ʈ ����
			mMonsterCombatScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
			EffectScript* effect = CreatureScript::callEffect();
			effect->SetEffectPosition(mCreatureTransform->GetPosition() + math::Vector3(0.0f, -0.20f, 0.0f));
			effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			effect->PlayEffect(L"Dying");
		}
		mIsDead = true;
	}

	void SkelScript::ChangeState(eMonsterState state)
	{
		mMonsterActiveState = state;
		switch (mMonsterActiveState)
		{
		case da::eMonsterState::Idle:
			mCreatureAnimator->PlayAnimation(L"SkelIdle");
			break;
		case da::eMonsterState::Chase:
			mCreatureAnimator->PlayAnimation(L"SkelMove");
			break;
		case da::eMonsterState::Attack:
			mCreatureAnimator->PlayAnimation(L"SkelIdle");
			break;
		case da::eMonsterState::Dead:
			GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
			break;
		default:
			break;
		}
	}
#pragma endregion
	
}