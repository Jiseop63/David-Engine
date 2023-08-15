#include "daSkelScript.h"
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
	{
	}

	SkelScript::~SkelScript()
	{
	}

	void SkelScript::Initialize()
	{
		// ������Ʈ ��������
		CreatureScript::Initialize();
		
		// �浹ü �����ϱ�
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

		// �ִϸ��̼� �����ϱ�
		std::shared_ptr<Texture> weapon = Resources::Load<Texture>(L"EnemyGreatSword", L"..\\Resources\\Texture\\Monster\\Skel\\GreatSwordSpriteSheet.png");
		mCreatureAnimator->Create(L"SkelSwing", weapon, math::Vector2(0.0f, 0.0f), math::Vector2(51.0f, 49.0f), 16, math::Vector2(0.0f, 0.0f), 0.1f);

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		mCreatureAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mCreatureAnimator->PlayAnimation(L"SkelIdle");
	}

	void SkelScript::Update()
	{
		SkelFSM();
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eCreatureState::Dead);
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
			mCreatureAnimator->PlayAnimation(L"SkelAttact");
			break;
		case da::eCreatureState::Dead:
			GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
			break;
		default:
			break;
		}
	}

	void SkelScript::SkelFSM()
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
		// Ž���� �ʿ��� ������
		//float detectYRange = 2.50f;
		//mCreatureStat.DetectRange;
		//
		//// ��ġ ���ϱ�
		//math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		//math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		//// �÷��̾ Ž��
		//float distanceX = skelPosition.x - playerPosition.x;
		//float distanceY = skelPosition.y - playerPosition.y;

		//if (abs(distanceX) >= mCreatureStat.DetectRange
		//	&& abs(distanceY) >= detectYRange)
		//{
		//	ChangeState(eCreatureState::Chase);
		//}
	}

	void SkelScript::SkelHandleChase()
	{
		math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		math::Vector3 skelPosition = mCreatureTransform->GetPosition();

		float distance = math::Vector3::Distance(skelPosition, playerPosition);
		// �÷��̾ Ž��
		float distanceX = skelPosition.x - playerPosition.x;
		float distanceY = skelPosition.y - playerPosition.y;

		// ���� �������� ����
		if (distance >= mCreatureStat.AttackRange)
		{
			ChangeState(eCreatureState::Attack);
		}

		// �ǵ��ƿͼ� Idle
	}

	void SkelScript::SkelHandleAttack()
	{
		// DoAttack!
		
		// retChase
	}

	void SkelScript::SkelHandleDead()
	{
		if (!mIsDead)
		{
			// ��� ����Ʈ ����
			mCreatureWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
			mEnemyEffectScript->SetEffectPosition(mCreatureTransform->GetPosition() + math::Vector3(0.0f, -0.20f, 0.0f));
			mEnemyEffectScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			mEnemyEffectScript->PlayEffect(L"Dying");
		}
		mIsDead = true;
	}
}