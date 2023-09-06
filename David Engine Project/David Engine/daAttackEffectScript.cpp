#include "daAttackEffectScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daPlayerCombatScript.h"

namespace da
{
	AttackEffectScript::AttackEffectScript()
		: mReqWeapon(nullptr)
	{
	}
	AttackEffectScript::~AttackEffectScript()
	{
	}
	void AttackEffectScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(2.50f, 2.50f, 1.0f);
		mEffectAnimator->Create(L"GreatSwing", Resources::Find<Texture>(L"GreatSwingFX"), math::Vector2::Zero, math::Vector2(32.0f, 25.0f), 4, math::Vector2::Zero, 0.0750f);
		mEffectAnimator->Create(L"Slash", Resources::Find<Texture>(L"SlashFX"), math::Vector2::Zero, math::Vector2(42.0f, 5.0f), 4, math::Vector2::Zero, 0.0750f, 25.0f);
		// 종료 이벤트 넣어주기
		mEffectAnimator->CompleteEvent(L"GreatSwing") = std::bind(&AttackEffectScript::retInactive, this);
		mEffectAnimator->CompleteEvent(L"Slash") = std::bind(&AttackEffectScript::retInactive, this);
	}
	void AttackEffectScript::PlayEffect(enums::eWeaponName weaponType)
	{
		switch (weaponType)
		{
		case da::enums::eWeaponName::Default:
			break;
		case da::enums::eWeaponName::LongSword:
		{
			mEffectTransform->SetScale(2.50f, 2.50f, 1.0f);
			MeleeEffect(L"GreatSwing");
		}
			break;
		default:
			break;
		}
	}
	void AttackEffectScript::PlayEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
	void AttackEffectScript::MeleeEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
	void AttackEffectScript::retInactive()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
}
