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
		// �̵�
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
		// Ȱ��ȭ �ǰ�
		if (mWeaponStat.ProjectileActive)
		{
			mWeaponStat.ProjectileAccumulate += (float)Time::DeltaTime();
			// �����ð��� �ѱ��
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
			// ���� �ǰ� ȣ��
			mReqWeapon->CallHitEffect(bossScript->GetBossTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.150f);
		}

		if (enums::eLayerType::Creature == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// ����Ʈ ȣ��
			mReqWeapon->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(20.0f, 0.150f);
			// �ǰ� ȣ��
			creatureScript->OnDamaged();
			
		}		
	}
}
