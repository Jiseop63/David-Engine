#include "daSkellBossHandScript.h"
#include "daGameObject.h"
#include "daSceneManager.h"
#include "daResources.h"
#include "daAnimator.h"
#include "daTime.h"
#include "daInput.h"
namespace da
{
	SkellBossHandScript::SkellBossHandScript()
		: mHandTransform(nullptr)
		, mHandRenderer(nullptr)
		, mHandAnimator(nullptr)
		, mHandCollider(nullptr)
		, mPlayerScript(nullptr)
		, mHandState(eHandFSM::Idle)
		, misLeftHand(false)
		, mChasePlayer(false)
		, mMovePositionY(0.0f)

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
		mHandCollider->SetSize(math::Vector2(4.0f, 0.60f));
		mHandCollider->ApplyComponentUsing(false);

		mPlayerScript = SceneManager::GetPlayerScript();
		mHandTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		mHandAnimator->Create(L"SkellBossHandIdle", Resources::Find<Texture>(L"SkellBossHandIdle"), math::Vector2(0.0f, 0.0f), math::Vector2(57.0f, 69.0f), 10, math::Vector2(0.0f, 0.0f), 0.10f, 75.0f);
		mHandAnimator->Create(L"SkellBossHandAttack", Resources::Find<Texture>(L"SkellBossHandAttack"), math::Vector2(0.0f, 0.0f), math::Vector2(70.0f, 80.0f), 18, math::Vector2(0.0f, 0.0f), 0.10f, 75.0f);
		
		mHandAnimator->ActionEvent(L"SkellBossHandAttack", 7) = std::bind(&SkellBossHandScript::activeCollider, this);
		mHandAnimator->CompleteEvent(L"SkellBossHandAttack") = std::bind(&SkellBossHandScript::retIdle, this);

		mHandAnimator->PlayAnimation(L"SkellBossHandIdle");

	}
	void SkellBossHandScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::Q))
		{
			mHandAnimator->PlayAnimation(L"SkellBossHandAttack");
		}
	}
	void SkellBossHandScript::LateUpdate()
	{
		reverseTexture();
		updateMoveToPlayer();
	}
	void SkellBossHandScript::DoAttack()
	{
		findPlayer();
	}
	void SkellBossHandScript::updateMoveToPlayer()
	{
		if (!mChasePlayer)
			return;
		// 플레이어 위치 구하기
		float playerPositionY = mPlayerScript->GetCreatureTransform()->GetPosition().y;
		float handPositionY = mHandTransform->GetPosition().y;
		math::Vector3 handPosition = mHandTransform->GetPosition();
		float distance = fabs(playerPositionY - handPositionY);
		
		// 종료 조건
		if (distance <= 0.05)
		{
			mChasePlayer = false;
			shootLaser();			// 애니메이션과 충돌기능 활성화
		}

		// 이동하기
		handPosition.y += 4.50f * mMovePositionY * (float)Time::DeltaTime();
		mHandTransform->SetPosition(handPosition);
	}
	void SkellBossHandScript::findPlayer()
	{
		// 플레이어 방향 찾기 & 이동속도는 거리에 비례
		math::Vector3 playerPosition = mPlayerScript->GetCreatureTransform()->GetPosition();;
		math::Vector3 handPosition = mHandTransform->GetPosition();
		mMovePositionY = playerPosition.y - handPosition.y;
		// 이동 상태로 변경
		mChasePlayer = true;
	}
	void SkellBossHandScript::shootLaser()
	{
		activeCollider();
		mHandAnimator->PlayAnimation(L"SkellBossHandAttack");
		
	}
	void SkellBossHandScript::retIdle()
	{
		mHandAnimator->PlayAnimation(L"SkellBossHandIdle");
		mHandCollider->ApplyComponentUsing(false);
	}

	void SkellBossHandScript::activeCollider()
	{
		if (misLeftHand)
			mHandCollider->SetCenter(math::Vector2(5.0f, 0.0f));
		else
			mHandCollider->SetCenter(math::Vector2(-5.0f, 0.0f));
		mHandCollider->ApplyComponentUsing(true);
	}

	void SkellBossHandScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mHandCollider->ApplyComponentUsing(false);
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 최종 피해량
			float totalDamage = 5.0f;
			// 피격 호출
			creatureScript->OnDamaged(totalDamage);
		}
		// 플레이어와 충돌한 경우 피해를 주고 충돌 비활성 시킴
		
	}
}
