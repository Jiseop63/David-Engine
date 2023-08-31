#include "daEffectEnemyScript.h"
#include "daGameObject.h"
#include "daResources.h"
namespace da
{
	EffectEnemyScript::EffectEnemyScript()
	{
	}
	EffectEnemyScript::~EffectEnemyScript()
	{
	}
	void EffectEnemyScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(1.750f, 1.750f, 1.0f);
		mEffectAnimator->Create(L"Dying", Resources::Find<Texture>(L"DyingFX"), math::Vector2::Zero, math::Vector2(64.0f, 64.0f), 12, math::Vector2::Zero, 0.050f, 40.0f);

		// 종료 이벤트 넣어주기
		mEffectAnimator->CompleteEvent(L"Dying") = std::bind(&EffectEnemyScript::retInactive, this);
	}
	void EffectEnemyScript::PlayEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
}

