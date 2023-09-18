#include "daSkellBossScript.h"
#include "daResources.h"
#include "daTime.h"
#include "daGameObject.h"

#include "daCollider2D.h"
#include "daAnimator.h"
#include "daPlayerScript.h"
#include "daSceneManager.h"

namespace da
{
	SkellBossScript::SkellBossScript()
		: mBossTransform(nullptr)
		, mBossCollider(nullptr)
		, mBossAnimator(nullptr)
		, mPlayerScript(nullptr)
		, mBossActiveState(eBossState::Idle)
		, mBossPattern(eBossPattern::Barrage)
		, mAttackReady(false)
		, mAttackCoolDownDelayTime(0.0f)
		, mAttackCoolDownAccumulateTime(0.0f)
		, mLeftHandTurn(true)
		, mMaxLaserCount(0)
		, mCurLaserCount(0)
	{
	}
	SkellBossScript::~SkellBossScript()
	{
	}
	void SkellBossScript::Initialize()
	{
		mBossTransform = GetOwner()->GetTransform();
		mBossCollider = GetOwner()->AddComponent<Collider2D>();
		mBossAnimator = GetOwner()->AddComponent<Animator>();

		mPlayerScript = SceneManager::GetPlayerScript();
		// Tr
		mBossTransform->SetScale(math::Vector3(6.0f, 6.0f, 1.0f));
		// 보스 충돌체
		mBossCollider->ImBody();
		mBossCollider->SetDetectionType(Collider2D::eDetectionType::Default);
		mBossCollider->SetSize(math::Vector2(0.45f, 0.7f));
		// 보스 애니메이션
		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"BossSpriteSheet", L"..\\Resources\\Texture\\\\Skel\\SpriteSheet.png");
		mBossAnimator->Create(L"SkellBossIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(85.0f, 99.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mBossAnimator->Create(L"SkellBossAttact", texture, math::Vector2(0.0f, 99.0f), math::Vector2(85.0f, 128.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		mBossAnimator->Create(L"SkellBossDead", texture, math::Vector2(0.0f, 227.0f), math::Vector2(85.0f, 128.0f), 1, math::Vector2(0.0f, 0.0f), 0.10f, 150.0f);
		
		mBossAnimator->CompleteEvent(L"SkellBossAttact") = std::bind(&SkellBossScript::retIdle, this);

		mBossAnimator->PlayAnimation(L"SkellBossIdle");

		// 변수
		mAttackReady = false;
		mAttackCoolDownDelayTime = 3.0f;
		mMaxLaserCount = 3;
	}
	void SkellBossScript::Update()
	{
		attackCoolDown();
		BossFSM();
	}
	void SkellBossScript::ChangeState(eBossState state)
	{
		switch (state)
		{
		case da::SkellBossScript::eBossState::Idle:
			mBossAnimator->PlayAnimation(L"SkellBossIdle");
			break;
		case da::SkellBossScript::eBossState::Attack:
			mBossAnimator->PlayAnimation(L"SkellBossAttact",false);
			break;
		case da::SkellBossScript::eBossState::Dead:
			mBossAnimator->PlayAnimation(L"SkellBossDead");
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
		if (mAttackReady)
		{
			mAttackReady = false;
			ChangeState(eBossState::Attack);
		}
	}
	void SkellBossScript::SkellBossHandleAttack()
	{
	}
	void SkellBossScript::SkellBossHandleDead()
	{
	}
	void SkellBossScript::attackCoolDown()
	{
		if (mAttackReady)
			return;

		mAttackCoolDownAccumulateTime += (float)Time::DeltaTime();

		if (mAttackCoolDownDelayTime <= mAttackCoolDownAccumulateTime)
		{
			mAttackCoolDownAccumulateTime = 0.0f;
			mAttackReady = true;
		}			
	}
	void SkellBossScript::shootLaser()
	{
		



		
		if (0 < mCurLaserCount)
		{
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

	void SkellBossScript::retIdle()
	{
		ChangeState(eBossState::Idle);
	}
}