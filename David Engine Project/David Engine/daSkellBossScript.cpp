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
		// 보스 충돌체
		mCreatureBodyCollider->ImBody();
		mCreatureBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mCreatureBodyCollider->SetSize(math::Vector2(0.45f, 0.7f));
		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);
		// 보스 애니메이션
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\\\Skel\\SpriteSheet.png");
		mCreatureAnimator->Create(L"SkellBossIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 99.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttact", texture, math::Vector2(0.0f, 99.0f), math::Vector2(85.0f, 128.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossAttacking", Resources::Find<Texture>(L"SkellBossAttacking"), math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 128.0f), 4, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mCreatureAnimator->Create(L"SkellBossDead", texture, math::Vector2(0.0f, 227.0f), math::Vector2(85.0f, 128.0f), 1, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		
		mCreatureAnimator->CompleteEvent(L"SkellBossAttact") = std::bind(&SkellBossScript::attackingAnimation, this);
		mCreatureAnimator->PlayAnimation(L"SkellBossIdle");


		mMaxLaserCount = 3;
		mCurLaserCount = mMaxLaserCount;
		// 선딜 (공격 패턴 대기시간)
		mPrepareDurationTime = 5.50f;
		mPrepareDurationDecay = mPrepareDurationTime;

		// 후딜
		mReadyDurationTime = 6.250f;
		mReadyDurationDecay = mReadyDurationTime;

		// 레이저 호출 딜레이 (3번 호출됨)
		mLaserCallDelayTime = 2.250f;
		mLaserCallDelayDecay = mLaserCallDelayTime;

		// 투사체 호출 딜레이 (60개가 호출됨 ㄷㄷ)
		mProjectileCallDelayTime = 0.20f; // 4.5초동안 공격한다는 가정하에 60개가 화면에 나타날 예정
		mProjectileCallDelayDecay = mProjectileCallDelayTime;


		mBossProjectileInfo = new structs::sBossProjectileStat();

		mBossProjectileInfo->ProjectileRange = 5.5f;							// 최대거리
		mBossProjectileInfo->ProjectileSpeed = 4.50f;							// 속도
		mBossProjectileInfo->ProjectileDamage = 5.0f;


	}
	void SkellBossScript::Update()
	{
		// 패턴 조건 찾기 (둘 중 하나가 충족되면 끝)
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
		// 공격 선딜
		prepareForAttack();
		// 패턴 선택 및 공격
		doAttack();
		// 공격 후딜
		readyForAttackDelay();
	}
	void SkellBossScript::SkellBossHandleDead()
	{
	}
	void SkellBossScript::prepareForAttack()
	{
		// 선딜이 끝났다면 넘기기
		if (mPrepareAttack)
			return;

		// 선딜 진행
		mPrepareDurationDecay -= (float)Time::DeltaTime();
		if (0 >= mPrepareDurationDecay)
		{
			mPrepareDurationDecay = mPrepareDurationTime;	// 선딜 시간값 초기화
			mPrepareAttack = true;							// 공격, 후딜 조건 활성화
		}
	}
	void SkellBossScript::doAttack()
	{
		// 아직 선딜레이가 남았으면 넘기기
		if (!mPrepareAttack)
			return;

		// 공격하기 전이라면
		if (!mAttackProgress)
		{
			mAttackProgress = true;							// 다음 진행으로 넘기기

			// 공격 패턴 선택
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
				mReadyDurationDecay = mReadyDurationTime;	// 후딜 시간값 초기화
				mPrepareAttack = false;						// 선딜 조건 초기화
				mAttackProgress = false;					// 공격 애니메이션 호출 조건 초기화

				mGetDamageCount = 0;						// 투사체, 레이저 패턴 조건 변수
				mProjectileAttackOn = false;				// 투사체 공격 비활성화
				mLaserAttackOn = false;						// 레이저 공격 비활성화
				mCurLaserCount = mMaxLaserCount;			// 레이저 공격 횟수 초기화
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
		if (mProjectileAttackOn)	// 투사체 공격중이라면
			return;
		if (!mLaserAttackOn)		// 레이저 공격이 아니라면
			return;
		if (eBossState::Idle == mBossActiveState)
			return;
		
		
		// 레이저 호출횟수가 남아잉슴
		if (0 < mCurLaserCount)
		{
			// 딜레이 계산
			mLaserCallDelayDecay -= (float)Time::DeltaTime();
			
			if (0 >= mLaserCallDelayDecay) // 딜레이 충족
			{
				mLaserCallDelayDecay = mLaserCallDelayTime;	// 딜레이 시간 초기화

				// Hand의 레이저를 호출하고, 카운트 감소
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
		// 카운트를 다했다면? 레이저 종료
		else
			mLaserAttackOn = false;
	}

	void SkellBossScript::callProjectileAttack()
	{
		if (mLaserAttackOn)				// 레이저 공격중이라면
			return;
		if (!mProjectileAttackOn)		// 투사체 공격 시간이 아니라면
			return;
		if (eBossState::Idle == mBossActiveState)
			return;

		// 딜레이 계산
		{
			float deltaTime = (float)Time::DeltaTime();
			mProjectileCallDelayDecay -= deltaTime;
			mRotatePerSeconds += deltaTime / 1.50f;
			if (0 >= mProjectileCallDelayDecay) // 딜레이 충족
			{
				mProjectileCallDelayDecay = mProjectileCallDelayTime;	// 딜레이 시간 초기화

				// 투사체를 4갈래로 각각 호출해주기!
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
		// 투사체 호출텀에 제한이 있어야함
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