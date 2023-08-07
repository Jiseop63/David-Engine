#include "daEffectScript.h"
#include "daGameObject.h"
namespace da
{
	EffectScript::EffectScript()
		: mEffectTransform(nullptr)
		, mEffectRenderer(nullptr)
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
		mEffectRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mEffectAnimator = GetOwner()->AddComponent<Animator>();
	}
	void EffectScript::LateUpdate()
	{
		mEffectRenderer->SetReverse(mReverse);
	}
}
