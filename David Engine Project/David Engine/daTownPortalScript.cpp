#include "daTownPortalScript.h"
#include "daResources.h"
#include "daGameObject.h"

namespace da
{
	TownPortalScript::TownPortalScript()
	{
	}

	TownPortalScript::~TownPortalScript()
	{
	}

	void TownPortalScript::Initialize()
	{
		PortalScript::Initialize();
		// Ʈ������ ����
		mPortalTransform->SetScale(math::Vector3(5.50f, 5.50f, 1.0f));

		
		// ������ ����
		mPortalRenderer->SetMaterial(Resources::Find<Material>(L"NoneMaterial"));
		// �ִϸ��̼� ����
		mPortalAnimator->Create(L"DungeonPortalEat", Resources::Find<Texture>(L"DungeonEat")
			, math::Vector2::Zero, math::Vector2(117.0f, 85.0f), 28, math::Vector2::Zero, 0.10f, 200.0f);
		mPortalAnimator->CompleteEvent(L"DungeonPortalEat") = std::bind(&PortalScript::ChangeScene, this);
		SetSceneName(L"Scene_Dungeon1F_Stage1");

		// �浹ü ����
		mPortalBodyCollider->SetSize(math::Vector2(2.0f, 0.10f));
		mPortalBodyCollider->SetCenter(math::Vector2(0.0f, -0.90f));
	}

	void TownPortalScript::OnCollisionEnter(Collider2D* other)
	{
		GameObject* playerObject = other->GetOwner();
		// �÷��̾� ����
		playerObject->SetObjectStates(GameObject::eObjectState::Hide);
		// �� ��ġ�� �÷��̾� ��ġ�� ����
		math::Vector3 playerPos = playerObject->GetTransform()->GetPosition();
		math::Vector3 portalPos = mPortalTransform->GetPosition();
		portalPos.x = playerPos.x;
		mPortalTransform->SetPosition(portalPos);
		// �ִϸ��̼� ���
		mPortalRenderer->SetMaterial(Resources::Find<Material>(L"AnimationMaterial"));
		mPortalAnimator->PlayAnimation(L"DungeonPortalEat", false);
	}

}