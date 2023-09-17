#include "daBansheeScript.h"
#include "daSceneManager.h"


#include "daTime.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daBansheeCombatScript.h"
#include "daEffectEnemyScript.h"
#include "daPlayerScript.h"
namespace da
{
	BansheeScript::BansheeScript()
		: mMonsterActiveState(eBansheeState::Idle)
		, mChaseDurationTime(0.0f)
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
	BansheeScript::~BansheeScript()
	{
	}
	void BansheeScript::Initialize()
	{
		MonsterScript::Initialize();
		// ������Ʈ ��������
		GetOwner()->SetName(L"BansheeObject");
		SetName(L"BansheeScript");

		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);


		// ���� �ʱ�ȭ
		mCreatureStat.MaxHP = 25.0f;
		mCreatureStat.CurHP = 25.0f;

		mMonsterAttackStat.AttackRange = 7.0f;
		mMonsterAttackStat.AttackDelayAccumulateTime = 0.0f;
		mMonsterAttackStat.AttackDelay = 4.0f;

		//mCreatureStat.MoveSpeed = 1.50f;

		mChaseDurationTime = 1.50f;
		mChaseDurationDecay = mChaseDurationTime;
		mPrepareDurationTime = 1.750f;
		mPrepareDurationDecay = mPrepareDurationTime;
		mReadyDurationTime = 7.50f;
		mReadyDurationDecay = mReadyDurationTime;

		// �浹ü �����ϱ�
		// body & foot
		{
			mCreatureBodyCollider->SetName(L"BansheeBodyCollider");
			mCreatureBodyCollider->SetSize(math::Vector2(0.30f, 0.40f));
			mCreatureBodyCollider->SetCenter(math::Vector2(0.0f, -0.10f));			
		}
		// senser
		{
			mMonsterSensorCollider->SetName(L"BansheeSensorCollider");
			mMonsterSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
			//mMonsterSensorCollider->SetColliderShapeType(enums::eColliderShape::Circle);
			mMonsterSensorCollider->SetSize(math::Vector2(mMonsterAttackStat.AttackRange, mMonsterAttackStat.AttackRange));
		}

		// �ִϸ��̼� �����ϱ�

		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"BansheeSpriteSheet");
		mCreatureAnimator->Create(L"BansheeIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(20.0f, 22.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f, 60.0f);
		mCreatureAnimator->Create(L"BansheeAttact", texture, math::Vector2(0.0f, 22.0f), math::Vector2(20.0f, 22.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f, 60.0f);
		mCreatureAnimator->PlayAnimation(L"BansheeIdle");
		mCreatureAnimator->CompleteEvent(L"BansheeAttact") = std::bind(&BansheeScript::retIdle, this);

		mPlayerScript = SceneManager::GetPlayerScript();
	}
	void BansheeScript::Update()
	{
		skelFSM();
		lifeCheck();
	}
	void BansheeScript::LateUpdate()
	{
		CreatureScript::visualUpdate();
	}
	void BansheeScript::retIdle()
	{
		mCreatureAnimator->PlayAnimation(L"BansheeIdle");
	}
	void BansheeScript::calcTargetDir()
	{
		math::Vector3 bansheePosition = mCreatureTransform->GetPosition();
		math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		mDistanceFromPlayer = playerPosition.x - mCreatureTransform->GetPosition().x;
		mCreatureDir = math::Vector2(playerPosition.x - bansheePosition.x, playerPosition.y - bansheePosition.y);
	}
	void BansheeScript::lifeCheck()
	{
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eBansheeState::Dead);
	}
	void BansheeScript::ChangeState(eBansheeState state)
	{
		mMonsterActiveState = state;
		
		switch (mMonsterActiveState)
		{
		case da::eBansheeState::Idle:
			mCreatureAnimator->PlayAnimation(L"BansheeIdle");
			break;
		case da::eBansheeState::Attack:
			mCreatureAnimator->PlayAnimation(L"BansheeAttack", false);
			break;
		case da::eBansheeState::Dead:
			GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
			break;
		default:
			break;
		}
	}
	void BansheeScript::skelFSM()
	{
		switch (mMonsterActiveState)
		{
		case da::eBansheeState::Idle:
			BansheeHandleIdle();
			break;
		case da::eBansheeState::Attack:
			BansheeHandleAttack();
			break;
		case da::eBansheeState::Dead:
			BansheeHandleDead();
			break;
		default:
			break;
		}
	}
	void BansheeScript::BansheeHandleIdle()
	{
		if (mDetectPlayer && !mIsAttacked)
		{
			ChangeState(eBansheeState::Attack);
			mIsAttacked = true;
		}
	}
	void BansheeScript::BansheeHandleAttack()
	{
		// ���� �غ� ������
		prepareForAttack();
		// ���� ����
		doAttack();
		// �ĵ�����
		readyForAttackDelay();
	}
	void BansheeScript::BansheeHandleDead()
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
	void BansheeScript::findingPlayer()
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
				ChangeState(eBansheeState::Idle);
			}
		}
	}
	void BansheeScript::prepareForAttack()
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
	void BansheeScript::doAttack()
	{
		// ���� �������̰� �������� �ѱ��
		if (!mPrepareAttack)
			return;

		// �����ϱ� ���̶��
		if (!mAttackProgress)
		{
			mCreatureAnimator->PlayAnimation(L"BansheeIdle");	// �ִϸ��̼� ȣ��
			mMonsterCombatScript->ToDoAttack();						// ���� ��� ȣ��
			mAttackProgress = true;									// ���� �������� �ѱ��
			mIsAttacked = false;									// �ִϸ��̼� �ʱ�ȭ
		}
	}
	void BansheeScript::readyForAttackDelay()
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
				ChangeState(eBansheeState::Idle);			// ���� ����
			}
		}
	}
}