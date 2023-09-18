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
		// �÷��̾� ��ġ ���ϱ�
		float playerPositionY = mPlayerScript->GetCreatureTransform()->GetPosition().y;
		float handPositionY = mHandTransform->GetPosition().y;
		math::Vector3 handPosition = mHandTransform->GetPosition();
		float distance = fabs(playerPositionY - handPositionY);
		
		// ���� ����
		if (distance <= 0.05)
		{
			mChasePlayer = false;
			shootLaser();			// �ִϸ��̼ǰ� �浹��� Ȱ��ȭ
		}

		// �̵��ϱ�
		handPosition.y += 4.50f * mMovePositionY * (float)Time::DeltaTime();
		mHandTransform->SetPosition(handPosition);
	}
	void SkellBossHandScript::findPlayer()
	{
		// �÷��̾� ���� ã�� & �̵��ӵ��� �Ÿ��� ���
		math::Vector3 playerPosition = mPlayerScript->GetCreatureTransform()->GetPosition();;
		math::Vector3 handPosition = mHandTransform->GetPosition();
		mMovePositionY = playerPosition.y - handPosition.y;
		// �̵� ���·� ����
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

			// ���� ���ط�
			float totalDamage = 5.0f;
			// �ǰ� ȣ��
			creatureScript->OnDamaged(totalDamage);
		}
		// �÷��̾�� �浹�� ��� ���ظ� �ְ� �浹 ��Ȱ�� ��Ŵ
		
	}
}
