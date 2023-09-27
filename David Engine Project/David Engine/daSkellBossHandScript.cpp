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

	}
	void SkellBossHandScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::Q))
		{
			DoAttack();
		}
	}
	void SkellBossHandScript::LateUpdate()
	{
		reverseTexture();
		updateMoveToPlayer();
	}
	void SkellBossHandScript::DoAttack()
	{
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
		// ����ü ��������
		ActionUnitScript* actionUnit = mOwnerScript->CallProjectile();
		structs::sActionUnitInfo unitInfo = {};
		unitInfo.Scale = 10.50f;
		unitInfo.Time.End = 0.250f;
		actionUnit->SetUnitTypes(UnitUsageType::Default, UnitRenderType::JustRotate, UnitActionType::None);
		actionUnit->SetUnitInfo(unitInfo);
		actionUnit->SetNextAnimation(L"SkellBossLaser", false);

		math::Vector3 offsetVector;
		if (!misLeftHand)
		{
			unitInfo.RotateAngle = 3.140f;
			offsetVector = math::Vector3(-4.0f, 0.0f, 0.0f);
		}
		else
		{
			unitInfo.RotateAngle = 0.0f;
			offsetVector = math::Vector3(4.0f, 0.0f, 0.0f);
		}

		actionUnit->OnActive();
		math::Vector3 handPosition = mHandTransform->GetPosition();
		actionUnit->SetOverridePosition(handPosition + offsetVector);
	}
	void SkellBossHandScript::retIdle()
	{
		mHandAnimator->PlayAnimation(L"SkellBossHandIdle");
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
		int a = 0;
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			// �÷��̾�� �浹�����Ƿ� ���� �ִϸ��̼ǰ� ������ �߻�
			mChasePlayer = false;
			shootLaser();			// �ִϸ��̼ǰ� �浹��� Ȱ��ȭ
		}

		//if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
		//	&& other->IsBody())
		//{
		//	mHandCollider->ApplyComponentUsing(false);
		//	GameObject* creatureObj = other->GetOwner();
		//	CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

		//	// ���� ���ط�
		//	float totalDamage = 5.0f;
		//	// �ǰ� ȣ��
		//	creatureScript->OnDamaged(totalDamage);
		//}
		// �÷��̾�� �浹�� ��� ���ظ� �ְ� �浹 ��Ȱ�� ��Ŵ		
	}
}