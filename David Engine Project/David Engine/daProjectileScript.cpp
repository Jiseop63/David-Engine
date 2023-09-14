#include "daProjectileScript.h"
#include "daGameObject.h"
#include "daTime.h"
#include "daPlayerCombatScript.h"
#include "daPlayerScript.h"
#include "daCreatureScript.h"
#include "daSkellBossScript.h"
#include "daSceneManager.h"
#include "daGameDataManager.h"
namespace da
{
	ProjectileScript::ProjectileScript()
		: mProjectileTransform(nullptr)
		, mProjectileCollider(nullptr)
		, mWeaponTransform(nullptr)
		, mWeaponScript(nullptr)
		, mWeaponInfo{}
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
		projectileProcess();
	}
	void ProjectileScript::projectileProcess()
	{
		switch (mWeaponInfo.ProjectileStat.ProjectileType)
		{
		case da::enums::eProjectileType::Melee:
			meleeProcess();
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
	void ProjectileScript::meleeProcess()
	{
		outOfTime();
	}
	void ProjectileScript::rangeProcess()
	{
		// 이동
		math::Vector3 retPosition = mProjectileTransform->GetPosition();
		retPosition *= mProjectileDir * (float)Time::DeltaTime();
		mProjectileTransform->SetPosition(retPosition);

		bool turnOff = false;
		if (mWeaponInfo.ProjectileStat.ProjectileRange <= mBeginPosition.Length())
			turnOff = true;
		if (mWeaponInfo.ProjectileStat.ProjectileCollision)
			turnOff = true;

		if (turnOff)
		{
			ClearProjectile();
		}
	}
	void ProjectileScript::bodyProcess()
	{
		if (!mWeaponTransform)
			mWeaponTransform = SceneManager::GetPlayerScript()->GetOwner()->GetTransform();
		
		mProjectileTransform->SetPosition(mWeaponTransform->GetPosition());
		outOfTime();
	}
	void ProjectileScript::outOfTime()
	{
		// 활성화 되고
		if (mWeaponInfo.ProjectileStat.ProjectileActive)
		{
			mWeaponInfo.ProjectileStat.ProjectileAccumulate += (float)Time::DeltaTime();
			// 유지시간을 넘기면
			if (mWeaponInfo.ProjectileStat.ProjectileAccumulate >= mWeaponInfo.ProjectileStat.ProjectileValidTime)
			{
				ClearProjectile();
			}
		}
	}

	void ProjectileScript::ClearProjectile()
	{
		mWeaponInfo.ProjectileStat.ProjectileAccumulate = 0.0f;
		mWeaponInfo.ProjectileStat.ProjectileCollision = false;
		mWeaponInfo.ProjectileStat.ProjectileActive = false;
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponInfo.ProjectileStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// 보스 피격 호출
			mWeaponScript->CallHitEffect(bossScript->GetBossTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);
		}
		if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponInfo.ProjectileStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// 이펙트 호출
			mWeaponScript->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);

			// 최종 피해량
			float totalDamage = 5.0f;


			// 피격 호출
			creatureScript->OnDamaged(totalDamage);
			
		}		
	}
}
