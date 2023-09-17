#include "daEffectScript.h"
#include "daGameObject.h"
#include "daTime.h"
#include "daCombatScript.h"
#include "daCreatureScript.h"

namespace da
{
	EffectScript::EffectScript()
		: mEffectTransform(nullptr)
		, mEffectRenderer(nullptr)
		, mEffectAnimator(nullptr)
		, mReverse(false)
		, mEffectMoveType(eEffectMoveType::DontMove)
		, mEffectDir(math::Vector3::Zero)
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
	void EffectScript::Update()
	{
		if (eEffectMoveType::DontMove != mEffectMoveType)
		{
			if (!mProjectileInfo->ProjectileActive)
				GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void EffectScript::LateUpdate()
	{
		mEffectRenderer->SetReverse(mReverse);

		switch (mEffectMoveType)
		{
		case da::EffectScript::DontMove:
			stayEffect();
			break;
		case da::EffectScript::Direction:
			directionEffect();
			break;
		case da::EffectScript::Rotate:
			rotateEffect();
			break;
		default:
			break;
		}
	}

	void EffectScript::PlayEffect(const std::wstring name)
	{
		mEffectAnimator->PlayAnimation(name, false);
	}
	void EffectScript::OnActive()
	{
		mBeginPosition = mCombatScript->GetOwnerScript()->GetCreatureTransform()->GetPosition();
		mEffectTransform->SetPosition(mBeginPosition);

		mEffectTransform->SetRotation(math::Vector3(0.0f, 0.0f, mProjectileInfo->ProjectileAngle));

		mProjectileInfo->ProjectileActive = true;
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		PlayEffect(mProjectileInfo->EffectName);
	}
	void EffectScript::stayEffect()
	{
	}
	void EffectScript::directionEffect()
	{
		math::Vector3 retPosition = mEffectTransform->GetPosition();
		retPosition += mProjectileInfo->ProjectileSpeed * mEffectDir * (float)Time::DeltaTime();
		mEffectTransform->SetPosition(retPosition);
	}
	void EffectScript::rotateEffect()
	{
		math::Vector3 retPosition = mEffectTransform->GetPosition();
		retPosition += mProjectileInfo->ProjectileSpeed * mEffectTransform->Up() * (float)Time::DeltaTime();
		mEffectTransform->SetPosition(retPosition);		
	}
}
