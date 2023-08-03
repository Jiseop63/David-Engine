#include "daEffectPlayerScript.h"
#include "daGameObject.h"
#include "daResources.h"
namespace da
{
	EffectPlayerScript::EffectPlayerScript()
	{
	}
	EffectPlayerScript::~EffectPlayerScript()
	{
	}
	void EffectPlayerScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(1.750f, 1.750f, 1.0f);
		mEffectAnimator->Create(L"Jumping", Resources::Find<Texture>(L"JumpFX"), math::Vector2::Zero, math::Vector2(9.0f, 10.0f), 6, math::Vector2::Zero, 0.050f, 50.0f);
		
		// ���� �̺�Ʈ �־��ֱ�
		mEffectAnimator->EndEvent(L"Jumping") = std::bind(&EffectScript::retInactive, this);
	}
	
}

