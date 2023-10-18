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
		// 보스 충돌체
		mCreatureBodyCollider->SetSize(math::Vector2(0.60f, 0.60f));
		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);
		
		mBossOpeningCollider = GetOwner()->AddComponent<Collider2D>();
		mBossOpeningCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
		mBossOpeningCollider->SetSize(math::Vector2(1.50f, 1.80f));
		// 보스 애니메이션
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
		
		// idle -> attack 으로 넘어가기 전 대기시간
		mIdleStayTime.SetTime(2.40f);

		// 투사체 공격이 유지되는 시간
		mProjectileFinishTime.SetTime(6.20f);
		mProjectileCallDelayTime.SetTime(0.250f); // 4.5초동안 공격한다는 가정하에 60개가 화면에 나타날 예정
		// 각 레이저 호출 딜레이 타임
		mLaserCallDelayTime.SetTime(0.50f);
		


		// 스텟 초기화
		mCreatureStat.MaxHP = 525.0f;
		mCreatureStat.CurHP = mCreatureStat.MaxHP;

	}
	void SkellBossScript::Update()
	{
		// 인트로 씬에서는 움직이면 안되니까 여기서 제어함
		if (!mIsControl)
			return;
		// 패턴 조건 찾기 (둘 중 하나가 충족되면 끝)
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

		// Attack 상태로 넘어가기전에 쿨다운 대기시간 넣기
		mIdleStayTime.Start += (float)Time::DeltaTime();
		if (mIdleStayTime.TimeOut())
		{
			mIdleStayTime.Clear();
			mBossActiveState = eBossState::Attack;
			mAttacking = true;
			if (mProjectileAttackOn)
				mCreatureAnimator->PlayAnimation(L"SkellBossAttact");
		}


		// 공격 준비가 되면 Attack 상태로 변경
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
		// 결정된 패턴을 시행하기
		if (mProjectileAttackOn)
		{
			// 패턴에 맞게 공격 시행
			callProjectileAttack();

			// 공격 종료 조건
			mProjectileFinishTime.Start += (float)Time::DeltaTime();
			if (mProjectileFinishTime.TimeOut())
			{
				// 조건 초기화
				mProjectileFinishTime.Clear();
				mGetDamageCount = 0;
				mProjectileAttackOn = false;
				mProjectileAttackActive = false;		// 특정 타이밍에 공격을 진행하고 싶어서 따로 변수로 조건체크
				mAttacking = false;

				// 상태 변경
				mCreatureAnimator->PlayAnimation(L"SkellBossIdle");					
				mBossActiveState = eBossState::Idle;
			}			
		}
		else
		{
			callLaserAttack();

			// 공격 종료 조건
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
			if (mProjectileCallDelayTime.TimeOut()) // 딜레이 충족
			{
				mProjectileCallDelayTime.Clear();	// 딜레이 시간 초기화
				mCreatureAudio->Play(Resources::Find<AudioClip>(L"RifleFire"), 10.0f, false);

				// 투사체를 4갈래로 각각 호출해주기!
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
		// 왼쪽이 공격할 차례임
		if (mLeftHandTurn)
		{
			// 반대편 공격이 끝남 (최초상태는 둘다 true임) // 일단 ok
			if (mRightHand->IsAttackFinished())
			{
				// 대기시간 됬음 (이것도 최초상태는 true임)
				if (mNextLaserAttackReady)
				{
					// 공격실행
					mLeftHand->DoAttack();
					mLeftHand->NotAttackFinish();	// 여기서 finish를 false로 세팅해놔도 공격이 끝나면 알아서 true가 될것임
					mLeftHandTurn = false;			// 반대편 손으로 턴을 넘김
					mNextLaserAttackReady = false;		// 대기시간을 초기화함 (방향이 바뀌면 다시 시간을 잴것임)
					mCurLaserCount--;				// 레이저 카운트도 감소시킴
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
				// 대기시간 됬음
				if (mNextLaserAttackReady)
				{
					// 공격실행
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

			// 손 치우기
			mRightHand->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
			mLeftHand->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);

			// 중력 활성화
			mCreatureRigidbody->ApplyComponentUsing(true);
			mCreatureFootCollider->ApplyComponentUsing(true);
			mCreatureBodyCollider->ApplyComponentUsing(false);
			// 사망 애니메이션
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