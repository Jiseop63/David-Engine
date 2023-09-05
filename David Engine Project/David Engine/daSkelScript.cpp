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

		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"SkelSprite");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");

		// 스텟 초기화
		mCreatureStat.MoveSpeed = 1.750f;

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
		// Chace 상태로 전환
		if (mDetectPlayer)
			ChangeState(eCreatureState::Chase);
	}

	void SkelScript::SkelHandleChase()
	{
		// 이동
		trackingPlayer();
		// 추격 제한시간 & 조건 달성시 공격 상태로 변경
		findingPlayer();
		
	}
	void SkelScript::trackingPlayer()
	{
		// 방향 구하기		
		calcCreatureDir();

		// 이동하기 *이동하기전에 벽 충돌 방향 확인하는 코드 필요함
		Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();				// 벽 충돌 체크
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();												// 내 위치
		float moveMagnitude = mCreatureStat.MoveSpeed * (float)Time::DeltaTime();									// 이동량
		math::Vector2 moveDir = daRotateVector2(mCreatureDir, mCreatureFootCollider->GetEnvRotate());				// 회전까지 고려한 이동방향
		math::Vector2 movePosition = moveDir * moveMagnitude;														// 이동위치
		skelPosition.x += movePosition.x;																			// 이동한 위치
		skelPosition.y += movePosition.y;																			// 이동한 위치

		// 벽 충돌 방향에 따른 이동 적용
		if (0 <= mCreatureDir.x)
		{
			if (Collider2D::eWallCollisionState::Right == wallCollisionState)
				return;
			mCreatureTransform->SetPosition(skelPosition);
		}
		else
		{
			if (Collider2D::eWallCollisionState::Left == wallCollisionState)
				return;
			mCreatureTransform->SetPosition(skelPosition);
		}

	}
	void SkelScript::findingPlayer()
	{
		// 플레이어가 인지범위 내에 있다면 or 없다면
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
		// 공격 범위에 도달했다면
		if (abs(mDistanceFromPlayer) <= mCreatureStat.AttackRange)
		{
			mChaseDurationDecay = mChaseDurationTime;
			ChangeState(eCreatureState::Attack);
		}
	}

	void SkelScript::SkelHandleAttack()
	{
		// 공격 준비 딜레이
		prepareForAttack();
		// 공격 진행
		doAttack();
		
		// 후딜레이
		readyForAttackDelay();

	}

	void SkelScript::prepareForAttack()
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
	void SkelScript::doAttack()
	{
		// 아직 선딜레이가 남았으면 넘기기
		if (!mPrepareAttack)
			return;

		// 공격하기 전이라면
		if (!mAttackProgress)
		{
			mCreatureAnimator->PlayAnimation(L"SkelIdle");	// 애니메이션 호출
			mCreatureWeaponScript->DoAttack();					// 공격 기능 호출
			mAttackProgress = true;								// 다음 진행으로 넘기기
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
				mReadyDurationDecay = mReadyDurationTime;	// 후딜 시간값 초기화
				mPrepareAttack = false;						// 선딜 조건 초기화
				mAttackProgress = false;					// 공격 애니메이션 호출 조건 초기화
				ChangeState(eCreatureState::Chase);			// 상태 변경
			}
		}
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