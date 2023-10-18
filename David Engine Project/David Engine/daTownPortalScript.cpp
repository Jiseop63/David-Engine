#include "daTownPortalScript.h"
#include "daResources.h"
#include "daGameObject.h"
#include "daSceneManager.h"

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
		mPortalAnimator->ActionEvent(L"DungeonPortalEat", 10) = std::bind(&TownPortalScript::HidePlayer, this);

		SetSceneName(L"Scene_dungeon1_1");

		// �浹ü ����
		mPortalBodyCollider->SetSize(math::Vector2(1.0f, 0.10f));
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
		SceneManager::GetLightObject()->GetComponent<AudioSource>()->Stop();		
		SceneManager::GetPlayerScript()->GetPlayerAudio()->Play(Resources::Find<AudioClip>(L"DungeonOut"), 80, false);

	}

	void TownPortalScript::HidePlayer()
	{
		mPortalTransform->SetPosition(mPortalTransform->GetPosition() + math::Vector3(0.0f, 0.0f, -1.0f));
	}
}