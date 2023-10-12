#include "daFairyScript.h"
#include "daGameObject.h"
#include "daResources.h"

namespace da
{
	FairyScript::FairyScript()
	{
	}
	FairyScript::~FairyScript()
	{
	}
	void FairyScript::Initialize()
	{
		mFairyTransform = GetOwner()->GetTransform();
		mFairyTransform->SetScale(math::Vector3(0.240f, 0.250f, 1.0f));
		mFairyCollider = GetOwner()->GetComponent<Collider2D>();
		mFairyAnimator = GetOwner()->GetComponent<Animator>();
		mFairyAnimator->Create(L"FairyIdle", Resources::Find<Texture>(L"FairyTexture"), math::Vector2::Zero, math::Vector2(24.0f, 25.0f), 16, math::Vector2::Zero, 0.1f);
		mFairyAnimator->PlayAnimation(L"FairyIdle");
	}
	void FairyScript::OnCollisionEnter(Collider2D* other)
	{
	}
}