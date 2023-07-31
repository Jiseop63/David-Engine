#include "daEffectScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTexture.h"
namespace da
{
	EffectScript::EffectScript()
		: mEffectTransform(nullptr)
		, mEffectAnimator(nullptr)
	{
	}
	EffectScript::~EffectScript()
	{
	}
	void EffectScript::Initialize()
	{
		mEffectTransform = GetOwner()->GetTransform();
		mEffectTransform->SetScale(2.50f, 2.50f, 1.0f);
		mEffectAnimator = GetOwner()->AddComponent<Animator>();
		mEffectAnimator->Create(L"GreatSwing", Resources::Find<Texture>(L"GreatSwingFX"), math::Vector2::Zero, math::Vector2(32.0f, 25.0f), 4, math::Vector2::Zero, 0.1f);
	}
	void EffectScript::Update()
	{
	}
	
	void EffectScript::PlayEffect()
	{
		mEffectAnimator->PlayAnimation(L"GreatSwing", false);
	}

}
