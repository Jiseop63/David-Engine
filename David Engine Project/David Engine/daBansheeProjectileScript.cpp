#include "daBansheeProjectileScript.h"
#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daCreatureScript.h"

namespace da
{
	BansheeProjectileScript::BansheeProjectileScript()
		: mChnaged(false)
	{
	}
	BansheeProjectileScript::~BansheeProjectileScript()
	{
	}
	void BansheeProjectileScript::Initialize()
	{
		ActionUnitScript::Initialize();
		mActionUnitAnimator->Create(L"BansheeBulletIdle", Resources::Find<Texture>(L"BansheeBulletIdle"), math::Vector2::Zero, math::Vector2(13.0f, 16.0f), 4, math::Vector2::Zero, 0.10f, 70.0f);
		mActionUnitAnimator->Create(L"BansheeBulletHit", Resources::Find<Texture>(L"BansheeBulletHit"), math::Vector2::Zero, math::Vector2(20.0f, 27.0f), 6, math::Vector2::Zero, 0.10f, 80.0f);
		
		// 종료 이벤트 넣어주기		
		mActionUnitAnimator->CompleteEvent(L"BansheeBulletHit") = std::bind(&ActionUnitScript::ClearUnit, this);

		mActionUnitCollider->SetSize(math::Vector2(0.30f, 0.30f));
	}
	void BansheeProjectileScript::Update()
	{
		// 종료 조건
		if (UnitRenderType::Stay == mUnitRenderType
			|| UnitRenderType::JustRotate == mUnitRenderType)
		{
			mUnitInfo.Time.Start += (float)Time::DeltaTime();
			if (mUnitInfo.Time.End <= mUnitInfo.Time.Start)
				ClearUnit();
		}
		else
		{
			math::Vector3 currentPosition = mActionUnitTransform->GetPosition();
			math::Vector3 moveDistance = mUnitBeginPosition - currentPosition;
			if (mUnitInfo.Range <= moveDistance.Length())
				if (!mChnaged)
					projectileHitEffect();
		}
		
	}

	void BansheeProjectileScript::projectileHitEffect()
	{
		math::Vector3 scale = mActionUnitTransform->GetScale();
		math::Vector3 totalscale = mActionUnitCollider->GetTotalScale();

		mChnaged = true;
		mUnitRenderType = UnitRenderType::Stay;
		mActionUnitAnimator->PlayAnimation(L"BansheeBulletHit", false);
	}
	void BansheeProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		// 플레이어와 충돌시
		GameObject* target = other->GetOwner();
		enums::eLayerType layerType = target->GetLayerType();
		if (enums::eLayerType::Playable == layerType)
		{
			CreatureScript* creatureScript = target->GetComponent<CreatureScript>();
			if (!mChnaged)
				projectileHitEffect();
		}
	}
	void BansheeProjectileScript::ClearUnit()
	{
		mChnaged = false;
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
}
