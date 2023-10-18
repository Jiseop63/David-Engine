#include "daSkellBossScript.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameObject.h"

#include "daGameDataManager.h"

#include "daCollider2D.h"
#include "daAnimator.h"
#include "daPlayerScript.h"
#include "daSceneManager.h"
#include "daSkellBossProjectileScript.h"
#include "daSkellBossHandScript.h"
#include "daDungeonPortalScript.h"

namespace da
{
	SkellBossScript::SkellBossScript()
		: mPlayerScript(nullptr)
		, mBossActiveState(eBossState::Idle)
		, mLeftHand(nullptr)
		, mRightHand(nullptr)
		, mPlayerJumpCount(0)
		, mPlayerDashCount(0)
		, mPlayerFind(false)

		, mIdleStayTime()
		, mGetDamageCount(0)
		, mProjectileAttackOn(false)
		, mNextLaserAttackReady(true)
		, mLaserAttakFinished(false)
		
		, mLeftHandTurn(true)
		, mMaxLaserCount(0)
		, mCurLaserCount(0)
		

		, mLaserCallDelayTime{}
		, mProjectileCallDelayTime{}
		, mRotatePerSeconds(0.0f)
		, mDeadTrigger(true)
		, mAttacking(false)
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
		mCreatureBodyCollider->SetSize(math::Vector2(0.60f, 0.60f));
		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);
		
		mBossOpeningCollider = GetOwner()->AddComponent<Collider2D>();
		mBossOpeningCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
		mBossOpeningCollider->SetSize(math::Vector2(1.50f, 1.80f));
		// ���� �ִϸ��̼�
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\\\Skel\\SpriteSheet.png");
		mCreatureAnimator->Create(L"SkellBossIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 99.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttact", texture, math::Vector2(0.0f, 99.0f), math::Vector2(85.0f, 128.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttacking", Resources::Find<Texture>(L"SkellBossAttacking"), math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 128.0f), 4, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossDead", texture, math::Vector2(0.0f, 227.0f), math::Vector2(85.0f, 128.0f), 1, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		
		mCreatureAnimator->CompleteEvent(L"SkellBossAttact") = std::bind(&SkellBossScript::playAttackingAnimation, this);
		mCreatureAnimator->ActionEvent(L"SkellBossAttact", 4) = std::bind(&SkellBossScript::startProjectileAttack, this);
		mCreatureAnimator->PlayAnimation(L"SkellBossIdle");


		mMaxLaserCount = 3;
		mCurLaserCount = mMaxLaserCount;
		
		// idle -> attack ���� �Ѿ�� �� ���ð�
		mIdleStayTime.SetTime(2.40f);

		// ����ü ������ �����Ǵ� �ð�
		mProjectileFinishTime.SetTime(6.20f);
		mProjectileCallDelayTime.SetTime(0.250f); // 4.5�ʵ��� �����Ѵٴ� �����Ͽ� 60���� ȭ�鿡 ��Ÿ�� ����
		// �� ������ ȣ�� ������ Ÿ��
		mLaserCallDelayTime.SetTime(0.50f);
		


		// ���� �ʱ�ȭ
		mCreatureStat.MaxHP = 525.0f;
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
		lifeCheck();
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
		if (!mPlayerFind)
			return;

		// Attack ���·� �Ѿ������ ��ٿ� ���ð� �ֱ�
		mIdleStayTime.Start += (float)Time::DeltaTime();
		if (mIdleStayTime.TimeOut())
		{
			mIdleStayTime.Clear();
			mBossActiveState = eBossState::Attack;
			mAttacking = true;
			if (mProjectileAttackOn)
				mCreatureAnimator->PlayAnimation(L"SkellBossAttact");
		}


		// ���� �غ� �Ǹ� Attack ���·� ����
		//if (!mPrepareAttack)
			
	}

	void SkellBossScript::patternCondition()
	{
		if (mAttacking)
			return;

		if (5 <= mGetDamageCount)
			mProjectileAttackOn = true;
	}

	void SkellBossScript::SkellBossHandleAttack()
	{
		// ������ ������ �����ϱ�
		if (mProjectileAttackOn)
		{
			// ���Ͽ� �°� ���� ����
			callProjectileAttack();

			// ���� ���� ����
			mProjectileFinishTime.Start += (float)Time::DeltaTime();
			if (mProjectileFinishTime.TimeOut())
			{
				// ���� �ʱ�ȭ
				mProjectileFinishTime.Clear();
				mGetDamageCount = 0;
				mProjectileAttackOn = false;
				mProjectileAttackActive = false;		// Ư�� Ÿ�ֿ̹� ������ �����ϰ� �; ���� ������ ����üũ
				mAttacking = false;

				// ���� ����
				mCreatureAnimator->PlayAnimation(L"SkellBossIdle");					
				mBossActiveState = eBossState::Idle;
			}			
		}
		else
		{
			callLaserAttack();

			// ���� ���� ����
			if (0 >= mCurLaserCount)
			{
				mGetDamageCount = 0;
				mLaserAttakFinished = false;
				mAttacking = false;
				mCurLaserCount = mMaxLaserCount;
			}
		}		
	}

	void SkellBossScript::callProjectileAttack()
	{
		if (mProjectileAttackActive)
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
					ActionUnitScript* actionUnit = CallBossActionUnit();
					actionUnit->SetUnitPosition(mCreatureTransform->GetPosition());

					structs::sActionUnitTypes mMonsterUnitTypes = {};
					mMonsterUnitTypes.Usage = enums::eUnitUsageType::AnimationProjectile;
					mMonsterUnitTypes.LifeCycle = enums::eUnitLifeType::Range;
					mMonsterUnitTypes.Action = enums::eUnitActionType::UsingDirection;
					actionUnit->SetUnitTypes(mMonsterUnitTypes);

					structs::sActionUnitStat mMonsterUnitInfo = {};
					mMonsterUnitInfo.Speed = 4.50f;
					mMonsterUnitInfo.Range = 6.5f;
					mMonsterUnitInfo.Animation.Idle = L"SkellBossProjectile";
					mMonsterUnitInfo.AtaackDamage = 1.250f;
					actionUnit->SetUnitInfo(mMonsterUnitInfo);
					actionUnit->SetUnitScale(math::Vector3(1.250f, 1.250f, 1.0f));
					actionUnit->SetUnitColliderSize(math::Vector2(0.40f, 0.40f));

					math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, projectile * 1.570f + mRotatePerSeconds);
					actionUnit->SetUnitDirection(moveDirection);
					actionUnit->SetUnitOffset(math::Vector3(0.0f, -0.90f, 0.0f));

					actionUnit->OnActive();
				}
			}
		}
	}
	void SkellBossScript::callLaserAttack()
	{
		// ������ ������ ������
		if (mLeftHandTurn)
		{
			// �ݴ��� ������ ���� (���ʻ��´� �Ѵ� true��) // �ϴ� ok
			if (mRightHand->IsAttackFinished())
			{
				// ���ð� ���� (�̰͵� ���ʻ��´� true��)
				if (mNextLaserAttackReady)
				{
					// ���ݽ���
					mLeftHand->DoAttack();
					mLeftHand->NotAttackFinish();	// ���⼭ finish�� false�� �����س��� ������ ������ �˾Ƽ� true�� �ɰ���
					mLeftHandTurn = false;			// �ݴ��� ������ ���� �ѱ�
					mNextLaserAttackReady = false;		// ���ð��� �ʱ�ȭ�� (������ �ٲ�� �ٽ� �ð��� �����)
					mCurLaserCount--;				// ������ ī��Ʈ�� ���ҽ�Ŵ
				}
				else
				{
					mLaserCallDelayTime.Start += (float)Time::DeltaTime();
					if (mLaserCallDelayTime.TimeOut())
					{
						mLaserCallDelayTime.Clear();
						mNextLaserAttackReady = true;
					}
				}
			}
		}
		else
		{
			if (mLeftHand->IsAttackFinished())
			{
				// ���ð� ����
				if (mNextLaserAttackReady)
				{
					// ���ݽ���
					mRightHand->DoAttack();
					mRightHand->NotAttackFinish();
					mLeftHandTurn = true;
					mNextLaserAttackReady = false;
					mCurLaserCount--;
				}
				else
				{
					mLaserCallDelayTime.Start += (float)Time::DeltaTime();
					if (mLaserCallDelayTime.TimeOut())
					{
						mLaserCallDelayTime.Clear();
						mNextLaserAttackReady = true;
					}
				}
			}
		}

		
	}


	void SkellBossScript::SkellBossHandleDead()
	{
		if (mDeadTrigger)
		{
			GameDataManager::DecreaseMonsterCount(SceneManager::GetActiveScene()->GetPortals());

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
			mCreatureFootCollider->SetCenter(math::Vector2(0.0f, -0.80f));
			mDeadTrigger = false;
			//SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();
		}
	}
	

	void SkellBossScript::AddActionUnit(GameObject* unit)
	{
		SkellBossProjectileScript* bossProjectile = unit->AddComponent<SkellBossProjectileScript>();
		bossProjectile->SetOwnerScript(this);
		mBossProjectiles.push_back(bossProjectile);
	}

	SkellBossProjectileScript* SkellBossScript::CallBossActionUnit()
	{
		for (size_t projectile = 0; projectile < mBossProjectiles.size(); ++projectile)
		{
			if (GameObject::eObjectState::Inactive ==
				mBossProjectiles[projectile]->GetOwner()->GetObjectState())
				return mBossProjectiles[projectile];
		}
		return nullptr;
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
			mBossActiveState = eBossState::Dead;
	}

	void SkellBossScript::retIdle()
	{
		mCreatureAnimator->PlayAnimation(L"SkellBossIdle");
		mBossActiveState = eBossState::Idle;
	}
	void SkellBossScript::playAttackingAnimation()
	{
		mCreatureAnimator->PlayAnimation(L"SkellBossAttacking");
	}
	void SkellBossScript::startProjectileAttack()
	{
		mProjectileAttackActive = true;
	}
}