#include "daSkelScript.h"
#include "daResources.h"
#include "daGameObject.h"

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
			mBodyCollider->SetSize(math::Vector2(0.30f, 0.40f));
			mBodyCollider->SetCenter(math::Vector2(0.0f, -0.10f));
		}
		{
			// foot
			mFootCollider->SetSize(math::Vector2(0.050f, 0.050f));
			mFootCollider->SetCenter(math::Vector2(0.0f, -0.450f));
			mFootCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
		}

		// �ִϸ��̼� �����ϱ�
		std::shared_ptr<Texture> weapon = Resources::Load<Texture>(L"EnemyGreatSword", L"..\\Resources\\Texture\\Monster\\Skel\\GreatSwordSpriteSheet.png");
		mAnimator->Create(L"SkelSwing", weapon, math::Vector2(0.0f, 0.0f), math::Vector2(51.0f, 49.0f), 16, math::Vector2(0.0f, 0.0f), 0.1f);

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		mAnimator->Create(L"SkelIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelMove", texture, math::Vector2(0.0f, 32.0f), math::Vector2(32.0f, 32.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelAttact", texture, math::Vector2(0.0f, 0.0f), math::Vector2(32.0f, 32.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->PlayAnimation(L"SkelIdle");
	}

	void SkelScript::HandleIdle()
	{
		// �÷��̾ Ž��
	}

	void SkelScript::HandleChase()
	{
		// �÷��̾� �������� �̵�
		// �ǵ��ƿͼ� Idle
	}

	void SkelScript::HandleAttack()
	{
		// ���ڸ����� �����ϱ�
		// ������ ������ �ٽ� Chase
	}

	void SkelScript::HandleDead()
	{
	}

	void SkelScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::PlayableAttackCollider == other->GetOwner()->GetLayerType())
		{
			// �ǰ� ȣ��
		}

	}

}