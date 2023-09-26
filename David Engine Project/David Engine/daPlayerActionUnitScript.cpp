#include "daPlayerActionUnitScript.h"
#include "daResources.h"
namespace da
{
	PlayerActionUnitScript::PlayerActionUnitScript()
	{
	}
	PlayerActionUnitScript::~PlayerActionUnitScript()
	{
	}
	void PlayerActionUnitScript::Initialize()
	{
		ActionUnitScript::Initialize();
		mActionUnitAnimator->Create(L"Jumping", Resources::Find<Texture>(L"JumpFX"), math::Vector2::Zero, math::Vector2(9.0f, 10.0f), 6, math::Vector2::Zero, 0.050f, 25.0f);
		mActionUnitAnimator->Create(L"DustEffect", Resources::Find<Texture>(L"DustFX"), math::Vector2::Zero, math::Vector2(16.0f, 16.0f), 7, math::Vector2::Zero, 0.050f, 30.0f);
		mActionUnitAnimator->Create(L"Dying", Resources::Find<Texture>(L"DyingFX"), math::Vector2::Zero, math::Vector2(64.0f, 64.0f), 12, math::Vector2::Zero, 0.050f, 40.0f);
		mActionUnitAnimator->CompleteEvent(L"Jumping") = std::bind(&ActionUnitScript::ClearUnit, this);
		mActionUnitAnimator->CompleteEvent(L"DustEffect") = std::bind(&ActionUnitScript::ClearUnit, this);
		mActionUnitAnimator->CompleteEvent(L"Dying") = std::bind(&ActionUnitScript::ClearUnit, this);

		mActionUnitAnimator->Create(L"GreatSwing", Resources::Find<Texture>(L"GreatSwingFX"), math::Vector2::Zero, math::Vector2(32.0f, 25.0f), 4, math::Vector2::Zero, 0.0750f);
		mActionUnitAnimator->Create(L"Swing", Resources::Find<Texture>(L"SwingFX"), math::Vector2::Zero, math::Vector2(40.0f, 28.0f), 3, math::Vector2::Zero, 0.0750f, 25.0f);
		mActionUnitAnimator->Create(L"Slash", Resources::Find<Texture>(L"SlashFX"), math::Vector2::Zero, math::Vector2(42.0f, 5.0f), 4, math::Vector2::Zero, 0.0750f, 25.0f);
		mActionUnitAnimator->CompleteEvent(L"GreatSwing") = std::bind(&ActionUnitScript::ClearUnit, this);
		mActionUnitAnimator->CompleteEvent(L"Swing") = std::bind(&ActionUnitScript::ClearUnit, this);
		mActionUnitAnimator->CompleteEvent(L"Slash") = std::bind(&ActionUnitScript::ClearUnit, this);

		mActionUnitAnimator->Create(L"SkellBossLaser", Resources::Find<Texture>(L"SkellBossLaser"), math::Vector2::Zero, math::Vector2(256.0f, 55.0f), 7, math::Vector2::Zero, 0.10f, 400.0f);
		mActionUnitAnimator->CompleteEvent(L"SkellBossLaser") = std::bind(&ActionUnitScript::ClearUnit, this);
	}
}