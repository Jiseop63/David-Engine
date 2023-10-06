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

		// ���� ����� ã��
		if (mTargetFind)
		{
			// ���� �������� ��ٸ�
			mAttackDelay.Start += (float)Time::DeltaTime();
			if (mAttackDelay.TimeOut())
			{
				// ������ �߻� �� ���� ��Ȱ��ȭ
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
		// �÷��̾� �߰� Ȱ��ȭ
		mChasePlayer = true;
		activeCollider();
	}
	void SkellBossHandScript::updateMoveToPlayer()
	{
		if (!mChasePlayer)
			return;
		// �÷��̾� ��ġ ���ϱ�
		math::Vector3 handPosition = mHandTransform->GetPosition();
		float handPositionY = handPosition.y;
		float playerPositionY = mPlayerScript->GetCreatureTransform()->GetPosition().y;
		
		// �̵��ϱ�
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
			// �÷��̾�� �浹�����Ƿ� �߰� �ߴ�
			mChasePlayer = false;

			// �߻簡 �غ�����Ƿ� update���� ���ǿ� ���� ���� ����
			mTargetFind = true;
		}
	}
}
