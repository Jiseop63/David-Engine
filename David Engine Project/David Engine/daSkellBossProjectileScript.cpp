#include "daSkellBossProjectileScript.h"
#include "daGameObject.h"
 
#include "daAnimator.h"
#include "daResources.h"
#include "daTime.h"

#include "daSkellBossScript.h"
#include "daCreatureScript.h"

namespace da
{
	SkellBossProjectileScript::SkellBossProjectileScript()
	{
	}
	SkellBossProjectileScript::~SkellBossProjectileScript()
	{
	}

	void SkellBossProjectileScript::Initialize()
	{
		ActionUnitScript::Initialize();
		
		//mProjectileTransform->SetScale(math::Vector3(1.50f, 1.50f, 1.0f));
		mActionUnitCollider->SetSize(math::Vector2(0.60f, 0.60f));

		// 애니메이션 초기화
		mActionUnitAnimator->Create(L"SkellBossProjectile", Resources::Find<Texture>(L"SkellBossProjectile"), math::Vector2::Zero, math::Vector2(50.0f, 50.0f), 10, math::Vector2::Zero, 0.10f, 80.0f);
		mActionUnitAnimator->Create(L"SkellBossLaser", Resources::Find<Texture>(L"SkellBossLaser"), math::Vector2::Zero, math::Vector2(256.0f, 55.0f), 7, math::Vector2::Zero, 0.10f, 400.0f);
		mActionUnitAnimator->CompleteEvent(L"SkellBossLaser") = std::bind(&ActionUnitScript::ClearUnit, this);
	}

	void SkellBossProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{	
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 피격 호출
			creatureScript->OnDamaged(7.0f);
			

			// 현재 패턴이 레이저 패턴이라면
			Animation* laserAnimation =  mActionUnitAnimator->FindAnimation(L"SkellBossLaser");
			Animation* currentAnimation = mActionUnitAnimator->GetActiveAnimation();
			if (laserAnimation == currentAnimation)
				mActionUnitCollider->ApplyComponentUsing(false);
			else
				this->ClearUnit();
		}
	}
}
