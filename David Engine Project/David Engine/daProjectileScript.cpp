#include "daProjectileScript.h"
#include "daGameObject.h"
#include "daTime.h"
#include "daWeaponScript.h"
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
		, mPlayerTransform(nullptr)
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
		projectileProcess();
	}
	void ProjectileScript::projectileProcess()
	{
		switch (mProjectileType)
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
		if (mWeaponStat.ProjectileRange <= mBeginPosition.Length())
			turnOff = true;
		if (mWeaponStat.ProjectileCollision)
			turnOff = true;

		if (turnOff)
		{
			mWeaponStat.ProjectileAccumulate = 0.0f;
			mWeaponStat.ProjectileCollision = false;
			mWeaponStat.ProjectileActive = false;
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void ProjectileScript::bodyProcess()
	{
		if (!mPlayerTransform)
			mPlayerTransform = SceneManager::GetPlayerScript()->GetOwner()->GetTransform();		
		
		mProjectileTransform->SetPosition(mPlayerTransform->GetPosition());
		outOfTime();
	}
	void ProjectileScript::outOfTime()
	{
		// 활성화 되고
		if (mWeaponStat.ProjectileActive)
		{
			mWeaponStat.ProjectileAccumulate += (float)Time::DeltaTime();
			// 유지시간을 넘기면
			if (mWeaponStat.ProjectileAccumulate >= mWeaponStat.ProjectileValidTime)
			{
				mWeaponStat.ProjectileAccumulate = 0.0f;
				mWeaponStat.ProjectileCollision = false;
				mWeaponStat.ProjectileActive = false;
				GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
			}
		}
	}

	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// 보스 피격 호출
			mReqWeapon->CallHitEffect(bossScript->GetBossTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.150f);
		}

		if (enums::eLayerType::Creature == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// 이펙트 호출
			mReqWeapon->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.150f);
			// 피격 호출
			creatureScript->OnDamaged();
			
		}		
	}
}
