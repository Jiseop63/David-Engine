#include "daSkellBossHandScript.h"
#include "daTime.h"
#include "daInput.h"
#include "daSceneManager.h"
#include "daResources.h"

#include "daGameObject.h"
#include "daAnimator.h"
#include "daSkellBossScript.h"
#include "daActionUnitScript.h"
#include "daSkellBossProjectileScript.h"
namespace da
{
	SkellBossHandScript::SkellBossHandScript()
		: mHandTransform(nullptr)
		, mHandRenderer(nullptr)
		, mHandAnimator(nullptr)
		, mHandCollider(nullptr)
		, mOwnerScript(nullptr)
		, mPlayerScript(nullptr)
		, mHandState(eHandFSM::Idle)
		, misLeftHand(false)
		, mChasePlayer(false)
		, mMovePositionY(0.0f)
		, mTargetFind(false)
		, mAttackReady(true)

	{
	}
	SkellBossHandScript::~SkellBossHandScript()
	{
	}
	void SkellBossHandScript::Initialize()
	{
		mHandTransform = GetOwner()->GetComponent<Transform>();
		mHandRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mHandAnimator = GetOwner()->AddComponent<Animator>();
		mHandCollider = GetOwner()->AddComponent<Collider2D>();
		mHandCollider->SetSize(math::Vector2(6.0f, 0.20f));
		mHandCollider->ApplyComponentUsing(false);

		mPlayerScript = SceneManager::GetPlayerScript();
		mHandTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		mHandAnimator->Create(L"SkellBossHandIdle", Resources::Find<Texture>(L"SkellBossHandIdle"), math::Vector2(0.0f, 0.0f), math::Vector2(57.0f, 69.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 75.0f);
		mHandAnimator->Create(L"SkellBossHandAttack", Resources::Find<Texture>(L"SkellBossHandAttack"), math::Vector2(0.0f, 0.0f), math::Vector2(70.0f, 80.0f), 18, math::Vector2(0.0f, 0.0f), 0.10f, 75.0f);
		
		//mHandAnimator->ActionEvent(L"SkellBossHandAttack", 7) = std::bind(&SkellBossHandScript::activeCollider, this);
		mHandAnimator->CompleteEvent(L"SkellBossHandAttack") = std::bind(&SkellBossHandScript::retIdle, this);

		mHandAnimator->PlayAnimation(L"SkellBossHandIdle");

		mAttackDelay.End = 0.50f;
		mAttackCooldown.End = 2.0f;
	}
	void SkellBossHandScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::Q))
		{
			DoAttack();
		}

		// 공격 대상을 찾음
		if (mTargetFind)
		{
			// 공격 선딜레이 기다림
			mAttackDelay.Start += (float)Time::DeltaTime();
			if (mAttackDelay.TimeOut())
			{
				// 레이저 발사 후 공격 비활성화
				shootLaser();
				mAttackDelay.Clear();
				mTargetFind = false;
			}
		}		
	}
	void SkellBossHandScript::LateUpdate()
	{
		reverseTexture();
		updateMoveToPlayer();
	}
	void SkellBossHandScript::DoAttack()
	{
		// 플레이어 추격 활성화
		mChasePlayer = true;
		activeCollider();
	}
	void SkellBossHandScript::updateMoveToPlayer()
	{
		if (!mChasePlayer)
			return;
		// 플레이어 위치 구하기
		math::Vector3 handPosition = mHandTransform->GetPosition();
		float handPositionY = handPosition.y;
		float playerPositionY = mPlayerScript->GetCreatureTransform()->GetPosition().y;
		
		// 이동하기
		float distance = playerPositionY - handPositionY;
		handPosition.y += 2.0f * distance * (float)Time::DeltaTime();
		mHandTransform->SetPosition(handPosition);
	}
	void SkellBossHandScript::shootLaser()
	{
		mHandAnimator->PlayAnimation(L"SkellBossHandAttack", false);
		mOwnerScript->GetCreatureAudio()->Play(Resources::Find<AudioClip>(L"jumperLaserFire"), 10.0f, false);

		ActionUnitScript* actionUnit = mOwnerScript->CallBossProjectile();

		structs::sUnitTypes effectUnitTypes = {};
		effectUnitTypes.ActionType = enums::eUnitActionType::None;
		effectUnitTypes.RenderType = enums::eUnitRenderType::Stay;
		effectUnitTypes.UsageType = enums::eUnitUsageType::Default;
		actionUnit->SetUnitTypes(effectUnitTypes);

		structs::sActionUnitInfo effectUnitInfo = {};
		actionUnit->SetUnitInfo(effectUnitInfo);
		actionUnit->SetUnitScale(math::Vector3(10.50f, 10.50f, 1.0f));

		structs::sAnimationInfo effectUnitAnimation = {};
		effectUnitAnimation.Name = L"SkellBossLaser";
		effectUnitAnimation.Loop = false;
		actionUnit->SetUnitAnimation(effectUnitAnimation);

		structs::sAttackStat effectAttackStat = {};
		effectAttackStat.AtaackDamage = 2.0f;
		actionUnit->SetUnitAttackStat(effectAttackStat);

		math::Vector3 offsetVector;
		if (misLeftHand)
			offsetVector = math::Vector3(4.0f, 0.0f, 0.0f);
		else
			offsetVector = math::Vector3(-4.0f, 0.0f, 0.0f);
		actionUnit->SetUnitReverse(!misLeftHand);

		actionUnit->SetUnitOffset(offsetVector);
		actionUnit->OnActive();
		math::Vector3 handPosition = mHandTransform->GetPosition();
		actionUnit->SetUnitOverridePosition(handPosition + offsetVector);
	}
	void SkellBossHandScript::retIdle()
	{
		mHandAnimator->PlayAnimation(L"SkellBossHandIdle");
		mTargetFind = false;
		mHandCollider->ApplyComponentUsing(false);
	}

	void SkellBossHandScript::activeCollider()
	{
		if (misLeftHand)
			mHandCollider->SetCenter(math::Vector2(4.0f, 0.0f));
		else
			mHandCollider->SetCenter(math::Vector2(-4.0f, 0.0f));
		mHandCollider->ApplyComponentUsing(true);
	}

	void SkellBossHandScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			// 플레이어와 충돌했으므로 추격 중단
			mChasePlayer = false;

			// 발사가 준비됬으므로 update에서 조건에 따라 공격 실행
			mTargetFind = true;
		}
	}
}
