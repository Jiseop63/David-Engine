#include "daFairyScript.h"
#include "daFairyScript.h"
#include "daSceneManager.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daPlayerScript.h"
namespace da
{
	FairyScript::FairyScript()
		: mFairyTransform(nullptr)
		, mFairyCollider(nullptr)
		, mFairyAnimator(nullptr)
	{
	}
	FairyScript::~FairyScript()
	{
	}
	void FairyScript::Initialize()
	{
		mFairyTransform = GetOwner()->GetTransform();
		mFairyTransform->SetScale(math::Vector3(1.0f, 1.0f, 1.0f));
		mFairyCollider = GetOwner()->AddComponent<Collider2D>();
		mFairyAnimator = GetOwner()->AddComponent<Animator>();
		mFairyAudio = GetOwner()->AddComponent<AudioSource>();
		mFairyAnimator->Create(L"FairyIdle", Resources::Find<Texture>(L"FairyTexture"), math::Vector2::Zero, math::Vector2(24.0f, 25.0f), 16, math::Vector2::Zero, 0.1f);
		mFairyAnimator->PlayAnimation(L"FairyIdle");
	}
	void FairyScript::OnCollisionEnter(Collider2D* other)
	{
		GameObject* creatureObj = other->GetOwner();
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType())
		{
			// ����� ä���� ȸ����Ŵ
			PlayerScript* player = creatureObj->GetComponent<PlayerScript>();
			player->GetHeal(10.0f);
			mFairyAudio->Play(Resources::Find<AudioClip>(L"Heal"), 175.0f, false);

			// ���� ��Ȱ��ȭ
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void FairyScript::DungeonFinished()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
}