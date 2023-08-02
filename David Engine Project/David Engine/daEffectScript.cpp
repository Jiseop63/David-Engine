#include "daEffectScript.h"
#include "daGameObject.h"
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
		mEffectAnimator = GetOwner()->AddComponent<Animator>();
	}
	
	void EffectScript::PlayEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}

}
