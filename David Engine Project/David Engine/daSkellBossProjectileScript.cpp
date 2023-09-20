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
		: mProjectileTransform(nullptr)
		, mProjectileRenderer(nullptr)
		, mProjectileAnimator(nullptr)
		, mProjectileCollider(nullptr)
		, mOwnerScript(nullptr)
		, mProjectileInfo(nullptr)
		, mBeginPosition(math::Vector3::Zero)
	{
	}
	SkellBossProjectileScript::~SkellBossProjectileScript()
	{
	}

	void SkellBossProjectileScript::Initialize()
	{
		// 컴포넌트 초기화
		mProjectileTransform = GetOwner()->GetComponent<Transform>();
		mProjectileRenderer = GetOwner()->GetComponent <MeshRenderer>();
		mProjectileCollider = GetOwner()->AddComponent<Collider2D>();
		mProjectileAnimator = GetOwner()->AddComponent<Animator>();
		
		mProjectileTransform->SetScale(math::Vector3(1.50f, 1.50f, 1.0f));
		mProjectileCollider->SetSize(math::Vector2(2.0f, 2.0f));

		// 애니메이션 초기화
		mProjectileAnimator->Create(L"SkellBossProjectile", Resources::Find<Texture>(L"SkellBossProjectile"), math::Vector2::Zero, math::Vector2(50.0f, 50.0f), 10, math::Vector2::Zero, 0.10f, 80.0f);
		mProjectileInfo = new structs::sBossProjectileStat();
	}

	void SkellBossProjectileScript::LateUpdate()
	{
		rangeProcess();
	}

	void SkellBossProjectileScript::OnActive()
	{
		mBeginPosition = mOwnerScript->GetOwner()->GetTransform()->GetPosition();
		mProjectileTransform->SetPosition(mBeginPosition);
		mProjectileTransform->SetRotation(math::Vector3(0.0f, 0.0f, mProjectileInfo->ProjectileAngle));

		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		mProjectileAnimator->PlayAnimation(L"SkellBossProjectile");
	}

	void SkellBossProjectileScript::playEffect(const std::wstring name)
	{
		mProjectileAnimator->PlayAnimation(name);
	}

	void SkellBossProjectileScript::rangeProcess()
	{
		math::Vector3 retPosition = mProjectileTransform->GetPosition();
		math::Vector3 projectileDir = math::daRotateVector3(math::Vector3::One, mProjectileInfo->ProjectileAngle);
		retPosition += mProjectileInfo->ProjectileSpeed * projectileDir * (float)Time::DeltaTime();
		mProjectileTransform->SetPosition(retPosition);


		bool turnOff = false;
		math::Vector3 betweenDistance = mBeginPosition - retPosition;
		// 사거리를 넘어감
		if (mProjectileInfo->ProjectileRange <= betweenDistance.Length())
			turnOff = true;
		// 플레이어와 충돌했다면
		if (mProjectileInfo->ProjectileCollision)						
			turnOff = true;

		if (turnOff)
			ClearProjectile();
	}

	void SkellBossProjectileScript::ClearProjectile()
	{
		mProjectileInfo->ProjectileCollision = false;
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void SkellBossProjectileScript::retInactive()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void SkellBossProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mProjectileInfo->ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// 피격 호출
			creatureScript->OnDamaged(mProjectileInfo->ProjectileDamage);
		}
	}

}
