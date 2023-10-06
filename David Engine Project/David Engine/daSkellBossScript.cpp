#include "daSkellBossScript.h"
#include "daSkellBossScript.h"
#include "daSkellBossScript.h"
#include "daSkellBossScript.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameObject.h"

#include "daCollider2D.h"
#include "daAnimator.h"
#include "daPlayerScript.h"
#include "daSceneManager.h"
#include "daSkellBossProjectileScript.h"
#include "daSkellBossHandScript.h"
namespace da
{
	SkellBossScript::SkellBossScript()
		: mPlayerScript(nullptr)
		, mBossActiveState(eBossState::Idle)
		, mAttackReady(false)
		
		, mLeftHandTurn(true)
		, mMaxLaserCount(0)
		, mCurLaserCount(0)
		
		, mPrepareAttack(false)
		, mPrepareDurationTime(0.0f)
		, mPrepareDurationDecay(0.0f)
		, mAttackProgress(false)
		, mReadyDurationTime(0.0f)
		, mReadyDurationDecay(0.0f)

		, mProjectileAttackOn(false)
		, mLaserAttackOn(false)
		, mLaserCallDelayTime{}
		, mProjectileCallDelayTime{}
		, mRotatePerSeconds(0.0f)
		, mDeadTrigger(false)
	{
	}
	SkellBossScript::~SkellBossScript()
	{
	}
	void SkellBossScript::Initialize()
	{
		CreatureScript::Initialize();
		
		mPlayerScript = SceneManager::GetPlayerScript();
		// Tr
		mCreatureTransform->SetScale(math::Vector3(5.50f, 5.50f, 1.0f));
		// ���� �浹ü
		mCreatureBodyCollider->ImBody();
		mCreatureBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureBodyCollider->SetSize(math::Vector2(0.60f, 0.60f));
		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);
		// ���� �ִϸ��̼�
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\\\Skel\\SpriteSheet.png");
		mCreatureAnimator->Create(L"SkellBossIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 99.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttact", texture, math::Vector2(0.0f, 99.0f), math::Vector2(85.0f, 128.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttacking", Resources::Find<Texture>(L"SkellBossAttacking"), math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 128.0f), 4, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossDead", texture, math::Vector2(0.0f, 227.0f), math::Vector2(85.0f, 128.0f), 1, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		
		mCreatureAnimator->CompleteEvent(L"SkellBossAttact") = std::bind(&SkellBossScript::attackingAnimation, this);
		mCreatureAnimator->PlayAnimation(L"SkellBossIdle");


		mMaxLaserCount = 3;
		mCurLaserCount = mMaxLaserCount;
		// ���� (���� ���� ���ð�)
		mPrepareDurationTime = 5.50f;
		mPrepareDurationDecay = mPrepareDurationTime;

		// �ĵ�
		mReadyDurationTime = 6.250f;
		mReadyDurationDecay = mReadyDurationTime;

		// ������ ȣ�� ������ (3�� ȣ���)
		mLaserCallDelayTime.End = 2.250f;
		// ����ü ȣ�� ������ (60���� ȣ��� ����)
		mProjectileCallDelayTime.End = 0.20f; // 4.5�ʵ��� �����Ѵٴ� �����Ͽ� 60���� ȭ�鿡 ��Ÿ�� ����

		// ���� �ʱ�ȭ
		mCreatureStat.MaxHP = 320.0f;
		mCreatureStat.CurHP = mCreatureStat.MaxHP;

	}
	void SkellBossScript::Update()
	{
		// ��Ʈ�� �������� �����̸� �ȵǴϱ� ���⼭ ������
		if (!mIsControl)
			return;
		// ���� ���� ã�� (�� �� �ϳ��� �����Ǹ� ��)
		patternCondition();
		BossFSM();
		callLaserAttack();
		callProjectileAttack();
		lifeCheck();
	}
	void SkellBossScript::ChangeStateAnimation(eBossState state)
	{
		switch (state)
		{
		case da::SkellBossScript::eBossState::Idle:
			mCreatureAnimator->PlayAnimation(L"SkellBossIdle");
			break;
		case da::SkellBossScript::eBossState::Attack:
			//mBossAnimator->PlayAnimation(L"SkellBossAttact",false);
			break;
		case da::SkellBossScript::eBossState::Dead:
			mCreatureAnimator->PlayAnimation(L"SkellBossDead");
			break;
		default:
			break;
		}
	}
	void SkellBossScript::BossFSM()
	{

		switch (mBossActiveState)
		{
		case da::SkellBossScript::eBossState::Idle:
			SkellBossHandleIdle();
			break;
		case da::SkellBossScript::eBossState::Attack:
			SkellBossHandleAttack();
			break;
		case da::SkellBossScript::eBossState::Dead:
			SkellBossHandleDead();
			break;
		default:
			break;
		}
	}
	void SkellBossScript::SkellBossHandleIdle()
	{
		if (!mPrepareAttack)
			mBossActiveState = eBossState::Attack;
	}
	void SkellBossScript::SkellBossHandleAttack()
	{
		// ���� ����
		prepareForAttack();
		// ���� ���� �� ����
		doAttack();
		// ���� �ĵ�
		readyForAttackDelay();
	}
	void SkellBossScript::SkellBossHandleDead()
	{
		if (mDeadTrigger)
		{
			// �� ġ���
			mRightHand->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
			mLeftHand->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);

			// �߷� Ȱ��ȭ
			mCreatureRigidbody->ApplyComponentUsing(true);
			mCreatureFootCollider->ApplyComponentUsing(true);
			mCreatureBodyCollider->ApplyComponentUsing(false);
			// ��� �ִϸ��̼�
			mCreatureTransform->SetScale(math::Vector3(5.0f, 5.0f, 1.0f));
			mCreatureAnimator->PlayAnimation(L"SkellBossDead");
			mCreatureFootCollider->SetSize(math::Vector2(0.10f, 0.10f));

			mDeadTrigger = false;
		}
	}
	void SkellBossScript::prepareForAttack()
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
	void SkellBossScript::doAttack()
	{
		// ���� �������̰� �������� �ѱ��
		if (!mPrepareAttack)
			return;

		// �����ϱ� ���̶��
		if (!mAttackProgress)
		{
			mAttackProgress = true;							// ���� �������� �ѱ��

			// ���� ���� ����
			if (mProjectileAttackOn)
			{
				mCreatureAnimator->PlayAnimation(L"SkellBossAttact");
			}
			else
				mLaserAttackOn = true;

		}
	}
	void SkellBossScript::readyForAttackDelay()
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

				mGetDamageCount = 0;						// ����ü, ������ ���� ���� ����
				mProjectileAttackOn = false;				// ����ü ���� ��Ȱ��ȭ
				mLaserAttackOn = false;						// ������ ���� ��Ȱ��ȭ
				mCurLaserCount = mMaxLaserCount;			// ������ ���� Ƚ�� �ʱ�ȭ
				mRotatePerSeconds = 0.0f;
				mBossActiveState = eBossState::Idle;
				mCreatureAnimator->PlayAnimation(L"SkellBossIdle");
			}
		}
	}

	void SkellBossScript::patternCondition()
	{
		if (8 <= mGetDamageCount
			&& !mLaserAttackOn)
			mProjectileAttackOn = true;
		else
			mProjectileAttackOn = false;
	}

	void SkellBossScript::AddActionUnit(GameObject* unit)
	{
		SkellBossProjectileScript* bossProjectile = unit->AddComponent<SkellBossProjectileScript>();
		bossProjectile->SetOwnerScript(this);
		mBossProjectiles.push_back(bossProjectile);
	}

	SkellBossProjectileScript* SkellBossScript::CallBossProjectile()
	{
		for (size_t projectile = 0; projectile < mBossProjectiles.size(); ++projectile)
		{
			if (GameObject::eObjectState::Inactive ==
				mBossProjectiles[projectile]->GetOwner()->GetObjectState())
				return mBossProjectiles[projectile];
		}
		return nullptr;
	}
	void SkellBossScript::callLaserAttack()
	{
		if (mProjectileAttackOn)	// ����ü �������̶�� ret
			return;
		if (!mLaserAttackOn)		// ������ ������ �ƴ϶�� ret
			return;
		if (eBossState::Idle == mBossActiveState)
			return;
		
		
		// ������ ȣ��Ƚ���� �����׽�
		if (0 < mCurLaserCount)
		{
			// ���� ���ð� ��ٸ���
			mLaserCallDelayTime.Start += (float)Time::DeltaTime();
			if (mLaserCallDelayTime.TimeOut())
			{
				// ȣ���� ��� ã��
				if (mLeftHandTurn)
				{
					// Hand�� �������� ȣ���ϰ�, ī��Ʈ ����
					mLeftHand->DoAttack();
					mLeftHandTurn = false;
					mCurLaserCount--;
				}
				else
				{
					mRightHand->DoAttack();
					mLeftHandTurn = true;
					mCurLaserCount--;
				}
				mLaserCallDelayTime.Clear();
			}								
		}
		// ī��Ʈ�� ���ߴٸ�? ������ ����
		else
			mLaserAttackOn = false;
	}

	void SkellBossScript::callProjectileAttack()
	{
		if (mLaserAttackOn)				// ������ �������̶��
			return;
		if (!mProjectileAttackOn)		// ����ü ���� �ð��� �ƴ϶��
			return;
		if (eBossState::Attack != mBossActiveState)
			return;

		// ������ ���
		{
			float deltaTime = (float)Time::DeltaTime();

			mProjectileCallDelayTime.Start += deltaTime;
			mRotatePerSeconds += deltaTime / 1.50f;
			if (mProjectileCallDelayTime.TimeOut()) // ������ ����
			{
				mProjectileCallDelayTime.Clear();	// ������ �ð� �ʱ�ȭ
				mCreatureAudio->Play(Resources::Find<AudioClip>(L"RifleFire"), 10.0f, false);
				
				// ����ü�� 4������ ���� ȣ�����ֱ�!
				for (size_t projectile = 0; projectile < 4; ++projectile)
				{
					ActionUnitScript* actionUnit = CallBossProjectile();

					structs::sUnitTypes mMonsterUnitTypes = {};
					mMonsterUnitTypes.ActionType = enums::eUnitActionType::Range;
					mMonsterUnitTypes.RenderType = enums::eUnitRenderType::UsingDirection;
					mMonsterUnitTypes.UsageType = enums::eUnitUsageType::Default;
					actionUnit->SetUnitTypes(mMonsterUnitTypes);

					structs::sActionUnitInfo mMonsterUnitInfo = {};
					mMonsterUnitInfo.Speed = 4.50f;
					mMonsterUnitInfo.Range = 6.5f;
					actionUnit->SetUnitInfo(mMonsterUnitInfo);
					actionUnit->SetUnitScale(math::Vector3(1.250f, 1.250f, 1.0f));
					actionUnit->SetUnitColliderSize(math::Vector2(0.70f, 0.70f));

					structs::sAnimationInfo mMonsterUnitAnimation = {};
					mMonsterUnitAnimation.Name = L"SkellBossProjectile";
					mMonsterUnitAnimation.Loop = true;
					actionUnit->SetUnitAnimation(mMonsterUnitAnimation);

					structs::sAttackStat mMonsterAttackStat = {};
					mMonsterAttackStat.AtaackDamage = 1.250f;
					actionUnit->SetUnitAttackStat(mMonsterAttackStat);

					math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, projectile * 1.570f + mRotatePerSeconds);
					actionUnit->SetUnitDirection(moveDirection);

					actionUnit->SetUnitOffset(math::Vector3(0.0f, -0.90f, 0.0f));
					actionUnit->OnActive();
				}

			}
		}
		// ����ü ȣ���ҿ� ������ �־����
	}

	void SkellBossScript::SetLeftHand(GameObject* left)
	{
		SkellBossHandScript* leftHandScript = left->AddComponent<SkellBossHandScript>();
		leftHandScript->SetLeftHand();
		leftHandScript->SetOwnerScript(this);
		mLeftHand = leftHandScript;
	}

	void SkellBossScript::SetRightHand(GameObject* right)
	{
		SkellBossHandScript* rightHandScript = right->AddComponent<SkellBossHandScript>();
		rightHandScript->SetOwnerScript(this);
		mRightHand = rightHandScript;
	}


	void SkellBossScript::lifeCheck()
	{
		if (0 >= mCreatureStat.CurHP)
		{
			mBossActiveState = eBossState::Dead;
			mDeadTrigger = true;
		}
			
	}

	void SkellBossScript::retIdle()
	{
		mCreatureAnimator->PlayAnimation(L"SkellBossIdle");
		mBossActiveState = eBossState::Idle;
	}
	void SkellBossScript::attackingAnimation()
	{
		mCreatureAnimator->PlayAnimation(L"SkellBossAttacking");
	}
}