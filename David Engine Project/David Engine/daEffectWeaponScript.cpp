#include "daEffectWeaponScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daWeaponScript.h"

namespace da
{
	EffectWeaponScript::EffectWeaponScript()
		: mReqWeapon(nullptr)
	{
	}
	EffectWeaponScript::~EffectWeaponScript()
	{
	}
	void EffectWeaponScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(2.50f, 2.50f, 1.0f);
		mEffectAnimator->Create(L"GreatSwing", Resources::Find<Texture>(L"GreatSwingFX"), math::Vector2::Zero, math::Vector2(32.0f, 25.0f), 4, math::Vector2::Zero, 0.0750f);
		mEffectAnimator->Create(L"Slash", Resources::Find<Texture>(L"SlashFX"), math::Vector2::Zero, math::Vector2(42.0f, 5.0f), 4, math::Vector2::Zero, 0.0750f, 35.0f);
		// 종료 이벤트 넣어주기
		mEffectAnimator->CompleteEvent(L"GreatSwing") = std::bind(&EffectWeaponScript::retInactive, this);
		mEffectAnimator->CompleteEvent(L"Slash") = std::bind(&EffectWeaponScript::retInactive, this);
	}
	void EffectWeaponScript::PlayEffect(enums::eWeaponName weaponType)
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
	void EffectWeaponScript::PlayEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
	void EffectWeaponScript::MeleeEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
	void EffectWeaponScript::retInactive()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
}
