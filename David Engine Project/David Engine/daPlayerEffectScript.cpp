#include "daPlayerEffectScript.h"
#include "daGameObject.h"
#include "daResources.h"
namespace da
{
	PlayerEffectScript::PlayerEffectScript()
	{
	}
	PlayerEffectScript::~PlayerEffectScript()
	{
	}
	void PlayerEffectScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(1.750f, 1.750f, 1.0f);
		mEffectAnimator->Create(L"Jumping", Resources::Find<Texture>(L"JumpFX"), math::Vector2::Zero, math::Vector2(9.0f, 10.0f), 6, math::Vector2::Zero, 0.050f, 25.0f);
		mEffectAnimator->Create(L"DustEffect", Resources::Find<Texture>(L"DustFX"), math::Vector2::Zero, math::Vector2(16.0f, 16.0f), 7, math::Vector2::Zero, 0.050f, 30.0f);
		mEffectAnimator->Create(L"Dying", Resources::Find<Texture>(L"DyingFX"), math::Vector2::Zero, math::Vector2(64.0f, 64.0f), 12, math::Vector2::Zero, 0.050f, 40.0f);
		
		// 종료 이벤트 넣어주기
		mEffectAnimator->CompleteEvent(L"Jumping") = std::bind(&PlayerEffectScript::retInactive, this);
		mEffectAnimator->CompleteEvent(L"DustEffect") = std::bind(&PlayerEffectScript::retInactive, this);
		mEffectAnimator->CompleteEvent(L"Dying") = std::bind(&PlayerEffectScript::retInactive, this);
	}
}

