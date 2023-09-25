#include "daSkellBossScript.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameObject.h"

#include "daCollider2D.h"
#include "daAnimator.h"
#include "daPlayerScript.h"
#include "daSceneManager.h"
#include "daSkellBossProjectileScript.h"

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
		, mLaserCallDelayTime(0.0f)
		, mLaserCallDelayDecay(0.0f)
		, mProjectileCallDelayTime(0.0f)
		, mProjectileCallDelayDecay(0.0f)
		, mRotatePerSeconds(0.0f)
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
		mCreatureTransform->SetScale(math::Vector3(6.0f, 6.0f, 1.0f));
		// ���� �浹ü
		mCreatureBodyCollider->ImBody();
		mCreatureBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureBodyCollider->SetSize(math::Vector2(0.45f, 0.7f));
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
		mLaserCallDelayTime = 2.250f;
		mLaserCallDelayDecay = mLaserCallDelayTime;

		// ����ü ȣ�� ������ (60���� ȣ��� ����)
		mProjectileCallDelayTime = 0.20f; // 4.5�ʵ��� �����Ѵٴ� �����Ͽ� 60���� ȭ�鿡 ��Ÿ�� ����
		mProjectileCallDelayDecay = mProjectileCallDelayTime;


		mBossProjectileInfo = new structs::sBossProjectileStat();

		mBossProjectileInfo->ProjectileRange = 5.5f;							// �ִ�Ÿ�
		mBossProjectileInfo->ProjectileSpeed = 4.50f;							// �ӵ�
		mBossProjectileInfo->ProjectileDamage = 5.0f;


	}
	void SkellBossScript::Update()
	{
		// ���� ���� ã�� (�� �� �ϳ��� �����Ǹ� ��)
		patternCondition();

		callLaserAttack();
		callProjectileAttack();
		BossFSM();
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
		if (6 <= mGetDamageCount)
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

	void SkellBossScript::AddProjectileObject(GameObject* projectile)
	{
		SkellBossProjectileScript* bossProjectile = projectile->AddComponent<SkellBossProjectileScript>();
		bossProjectile->SetOwnerScript(this);
		mBossProjectiles.push_back(bossProjectile);
	}
	SkellBossProjectileScript* SkellBossScript::callProjectile()
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
		if (mProjectileAttackOn)	// ����ü �������̶��
			return;
		if (!mLaserAttackOn)		// ������ ������ �ƴ϶��
			return;
		if (eBossState::Idle == mBossActiveState)
			return;
		
		
		// ������ ȣ��Ƚ���� �����׽�
		if (0 < mCurLaserCount)
		{
			// ������ ���
			mLaserCallDelayDecay -= (float)Time::DeltaTime();
			
			if (0 >= mLaserCallDelayDecay) // ������ ����
			{
				mLaserCallDelayDecay = mLaserCallDelayTime;	// ������ �ð� �ʱ�ȭ

				// Hand�� �������� ȣ���ϰ�, ī��Ʈ ����
				if (mLeftHandTurn)
				{
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
		if (eBossState::Idle == mBossActiveState)
			return;

		// ������ ���
		{
			float deltaTime = (float)Time::DeltaTime();
			mProjectileCallDelayDecay -= deltaTime;
			mRotatePerSeconds += deltaTime / 1.50f;
			if (0 >= mProjectileCallDelayDecay) // ������ ����
			{
				mProjectileCallDelayDecay = mProjectileCallDelayTime;	// ������ �ð� �ʱ�ȭ

				// ����ü�� 4������ ���� ȣ�����ֱ�!
				for (size_t projectile = 0; projectile < 4; ++projectile)
				{

					SkellBossProjectileScript* targetProjectile = callProjectile();

					mBossProjectileInfo->ProjectileDamage = 5.0f;

					structs::sActionUnitInfo unitInfo = {};
					unitInfo.Scale = 2.0f;
					unitInfo.Time.End = 2.0f;
					unitInfo.Range = 5.5f;
					unitInfo.Speed = 4.50f;
					math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, projectile * 1.570f + mRotatePerSeconds);
					targetProjectile->SetUnitInfo(unitInfo);
					targetProjectile->SetUnitTypes(UnitActionType::UsingDirection, UnitUsageType::Default);
					targetProjectile->SetNextAnimation(L"SkellBossProjectile", true);
					targetProjectile->SetMoveDirection(moveDirection);
					targetProjectile->OnActive();
				}				
			}			
		}
		// ����ü ȣ���ҿ� ������ �־����
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