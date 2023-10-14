#include "daFairyScript.h"
#include "daFairyScript.h"
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
		
		mFairyAnimator->Create(L"FairyIdle", Resources::Find<Texture>(L"FairyTexture"), math::Vector2::Zero, math::Vector2(24.0f, 25.0f), 16, math::Vector2::Zero, 0.1f);
		mFairyAnimator->PlayAnimation(L"FairyIdle");
	}
	void FairyScript::OnCollisionEnter(Collider2D* other)
	{
		GameObject* creatureObj = other->GetOwner();
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType())
		{
			// 대상의 채력을 회복시킴
			PlayerScript* player = creatureObj->GetComponent<PlayerScript>();
			player->GetHeal(10.0f);
			// 나를 비활성화
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void FairyScript::DungeonFinished()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
}