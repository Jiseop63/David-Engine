#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectPlayerScript : public EffectScript
	{
	public:
		EffectPlayerScript();
		virtual ~EffectPlayerScript();

		virtual void Initialize() override;

		virtual void PlayEffect(const std::wstring name) override;
		virtual void retInactive() override { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }
	};
}