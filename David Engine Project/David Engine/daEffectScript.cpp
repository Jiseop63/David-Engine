#include "daEffectScript.h"
#include "daGameObject.h"
namespace da
{
	EffectScript::EffectScript()
		: mEffectTransform(nullptr)
		, mEffectAnimator(nullptr)
		, mReverse(false)
	{
	}
	EffectScript::~EffectScript()
	{
	}
	void EffectScript::Initialize()
	{
		mEffectTransform = GetOwner()->GetTransform();
		mRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mEffectAnimator = GetOwner()->AddComponent<Animator>();
	}
	void EffectScript::LateUpdate()
	{
		mRenderer->SetReverse(mReverse);
	}
}
