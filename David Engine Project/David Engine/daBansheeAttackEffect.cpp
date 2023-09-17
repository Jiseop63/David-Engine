#include "daBansheeAttackEffect.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daBansheeCombatScript.h"
#include "daCreatureScript.h"

namespace da
{
	BansheeAttackEffectScript::BansheeAttackEffectScript()
	{
	}
	BansheeAttackEffectScript::~BansheeAttackEffectScript()
	{
	}
	void BansheeAttackEffectScript::Initialize()
	{
		EffectScript::Initialize();
		mEffectTransform->SetScale(2.50f, 2.50f, 1.0f);
		mEffectAnimator->Create(L"BansheeBulletIdle", Resources::Find<Texture>(L"BansheeBulletIdle"), math::Vector2::Zero, math::Vector2(13.0f, 16.0f), 4, math::Vector2::Zero, 0.10f, 70.0f);
		mEffectAnimator->Create(L"BansheeBulletHit", Resources::Find<Texture>(L"BansheeBulletHit"), math::Vector2::Zero, math::Vector2(20.0f, 27.0f), 6, math::Vector2::Zero, 0.10f, 80.0f);
		// 종료 이벤트 넣어주기		
		mEffectAnimator->CompleteEvent(L"BansheeBulletHit") = std::bind(&BansheeAttackEffectScript::retInactive, this);
	}
	void BansheeAttackEffectScript::Update()
	{
		if (eEffectMoveType::DontMove != mEffectMoveType)
		{
			if (!mProjectileInfo->ProjectileActive
				&& !mChanged)
			{
				projectileHitEffect();
				mChanged = true;
			}
		}
	}
	void BansheeAttackEffectScript::LateUpdate()
	{
		if (mProjectileInfo->ProjectileActive)
		{
			math::Vector3 retPosition = mEffectTransform->GetPosition();
			retPosition += mProjectileInfo->ProjectileSpeed * mEffectDir * (float)Time::DeltaTime();
			mEffectTransform->SetPosition(retPosition);
		}		
	}

	void BansheeAttackEffectScript::OnActive()
	{
		mChanged = false;
		mBeginPosition = mCombatScript->GetOwnerScript()->GetCreatureTransform()->GetPosition();
		mEffectTransform->SetPosition(mBeginPosition);

		mProjectileInfo->ProjectileActive = true;
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		mEffectAnimator->PlayAnimation(mProjectileInfo->EffectName, true);
	}

	void BansheeAttackEffectScript::retInactive()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void BansheeAttackEffectScript::projectileHitEffect()
	{
		mEffectAnimator->PlayAnimation(L"BansheeBulletHit", true);
	}
}
