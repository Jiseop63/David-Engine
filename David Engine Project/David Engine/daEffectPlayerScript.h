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

		void PlayEffect(const std::wstring name);
		void retInactive() { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }
	};
}