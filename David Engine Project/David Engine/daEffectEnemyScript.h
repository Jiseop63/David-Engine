#pragma once
#include "daEffectScript.h"

namespace da
{
	class EffectEnemyScript : public EffectScript
	{
	public:
		EffectEnemyScript();
		virtual ~EffectEnemyScript();

		virtual void Initialize() override;

		void PlayEffect(const std::wstring name);
		void retInactive() { GetOwner()->SetObjectState(GameObject::eObjectState::Inactive); }
	};
}