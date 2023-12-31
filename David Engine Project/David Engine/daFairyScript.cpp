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
		, mValue(0.0f)
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
		mValue = 10.0f;
	}
	void FairyScript::OnCollisionEnter(Collider2D* other)
	{
		GameObject* creatureObj = other->GetOwner();
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType())
		{
			// 대상의 채력을 회복시킴
			PlayerScript* player = creatureObj->GetComponent<PlayerScript>();
			player->GetHeal(mValue);
			mFairyAudio->Play(Resources::Find<AudioClip>(L"Heal"), 175.0f, false);

			// 나를 비활성화
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void FairyScript::DungeonFinished()
	{
		
	}
}