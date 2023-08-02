#include "daEffectPlayerScript.h"
#include "daGameObject.h"
#include "daResources.h"
namespace da
{
	EffectPlayerScript::EffectPlayerScript()
	{
	}
	EffectPlayerScript::~EffectPlayerScript()
	{
	}
	void EffectPlayerScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(1.50f, 1.50f, 1.0f);
		mEffectAnimator->Create(L"Jumping", Resources::Find<Texture>(L"JumpFX"), math::Vector2::Zero, math::Vector2(9.0f, 10.0f), 6, math::Vector2::Zero, 0.050f, 50.0f);
		
		// 종료 이벤트 넣어주기
		mEffectAnimator->CompleteEvent(L"Jumping") = std::bind(&EffectPlayerScript::retInactive, this);
	}
	void EffectPlayerScript::retInactive()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
}

