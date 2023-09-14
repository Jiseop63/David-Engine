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
		// �̵�
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
		// Ȱ��ȭ �ǰ�
		if (mWeaponInfo.ProjectileStat.ProjectileActive)
		{
			mWeaponInfo.ProjectileStat.ProjectileAccumulate += (float)Time::DeltaTime();
			// �����ð��� �ѱ��
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
			// ���� �ǰ� ȣ��
			mWeaponScript->CallHitEffect(bossScript->GetBossTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);
		}
		if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mWeaponInfo.ProjectileStat.ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// ����Ʈ ȣ��
			mWeaponScript->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);

			// ���� ���ط�
			float totalDamage = 5.0f;


			// �ǰ� ȣ��
			creatureScript->OnDamaged(totalDamage);
			
		}		
	}
}
