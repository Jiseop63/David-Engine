#include "daProjectileScript.h"
#include "daGameObject.h"
#include "daTime.h"
#include "daWeaponScript.h"
#include "daPlayerScript.h"
#include "daCreatureScript.h"
#include "daSceneManager.h"

namespace da
{
	ProjectileScript::ProjectileScript()
		: mProjectileTransform(nullptr)
		, mProjectileCollider(nullptr)
		, mReqWeapon(nullptr)
		, mProjectileType(enums::eProjectileType::Melee)
		, mIsCollision(false)
	{
	}
	ProjectileScript::~ProjectileScript()
	{
	}
	void ProjectileScript::Initialize()
	{
		mProjectileTransform = GetOwner()->GetTransform();
		mProjectileCollider = GetOwner()->AddComponent<Collider2D>();
	}
	void ProjectileScript::Update()
	{
		turnOffCollision();
		projectileProcess();
	}
	void ProjectileScript::projectileProcess()
	{
		switch (mProjectileType)
		{
		case da::enums::eProjectileType::Melee:
			break;
		case da::enums::eProjectileType::Range:
			rangeProcess();
			break;
		case da::enums::eProjectileType::Body:
			bodyProcess();
			break;
		default:
			break;
		}
	}
	void ProjectileScript::rangeProcess()
	{
		// 이동
		math::Vector3 retPosition = mProjectileTransform->GetPosition();
		retPosition *= mProjectileDir * (float)Time::DeltaTime();
		mProjectileTransform->SetPosition(retPosition);

		// 조건
		if (mIsCollision)
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void ProjectileScript::bodyProcess()
	{
		// 이동
		math::Vector3 retPosition = mReqWeapon->GetPlayerScript()->GetOwner()->GetTransform()->GetPosition();
		mProjectileTransform->SetPosition(retPosition);
	}
	void ProjectileScript::turnOffCollision()
	{
		if (!mReqWeapon->IsProjectileCollision())
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Creature == other->GetOwner()->GetLayerType())
		{
			// 피격 호출
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			creatureScript->OnDamaged();
			mReqWeapon->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(60.0f, 0.10f);
		}
	}
}
