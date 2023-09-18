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
		, mCombatScript(nullptr)
		, mProjectileInfo{}
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
	void ProjectileScript::OnActive()
	{
		mBeginPosition = mCombatScript->GetOwnerScript()->GetCreatureTransform()->GetPosition();
		mProjectileTransform->SetPosition(mBeginPosition);
		mProjectileTransform->SetRotation(math::Vector3(0.0f, 0.0f, mProjectileInfo->ProjectileAngle));
		mProjectileCollider->SetCenter(mCombatScript->GetOwnerScript()->GetCreatureDir() * mProjectileInfo->ProjectileCenterPadding);

		mProjectileInfo->ProjectileActive = true;
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void ProjectileScript::projectileProcess()
	{
		switch (mProjectileInfo->ProjectileType)
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
		retPosition += mProjectileInfo->ProjectileSpeed * mProjectileTransform->Up() * (float)Time::DeltaTime();
		mProjectileTransform->SetPosition(retPosition);


		math::Vector3 moveVector = mBeginPosition - retPosition;
		
		bool turnOff = false;
		if (mProjectileInfo->ProjectileRange <= moveVector.Length())
			turnOff = true;
		if (mProjectileInfo->ProjectileCollision)
			turnOff = true;

		if (turnOff)
		{
			ClearProjectile();
		}
	}
	void ProjectileScript::bodyProcess()
	{
		if (!mCombatScript->GetOwnerScript()->GetCreatureTransform())
			mProjectileTransform->SetPosition(mCombatScript->GetOwnerScript()->GetCreatureTransform()->GetPosition());
		outOfTime();
	}
	void ProjectileScript::outOfTime()
	{
		// Ȱ��ȭ �ǰ�
		if (mProjectileInfo->ProjectileActive)
		{
			mProjectileInfo->ProjectileValidAccumulateTime += (float)Time::DeltaTime();
			// �����ð��� �ѱ��
			if (mProjectileInfo->ProjectileValidAccumulateTime >= mProjectileInfo->ProjectileValidTime)
			{
				ClearProjectile();
			}
		}
	}

	void ProjectileScript::ClearProjectile()
	{
		mProjectileInfo->ProjectileValidAccumulateTime = 0.0f;
		mProjectileInfo->ProjectileCollision = false;
		mProjectileInfo->ProjectileActive = false;
		GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}

	void ProjectileScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Boss == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mProjectileInfo->ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			SkellBossScript* bossScript = creatureObj->GetComponent<SkellBossScript>();
			// ���� �ǰ� ȣ��
			mCombatScript->CallHitEffect(bossScript->GetBossTransform()->GetPosition());
			bossScript->IncreaseDamageCount();
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);	// ī�޶� ����
		}
		if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mProjectileInfo->ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();
			// ����Ʈ ȣ��
			mCombatScript->CallHitEffect(creatureScript->GetCreatureTransform()->GetPosition());
			SceneManager::GetMainCameraScript()->SetOscillation(120.0f, 0.1250f);

			// ���� ���ط�
			float totalDamage = 5.0f;


			// �ǰ� ȣ��
			creatureScript->OnDamaged(totalDamage);			
		}
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			mProjectileInfo->ProjectileCollision = true;
			GameObject* creatureObj = other->GetOwner();
			CreatureScript* creatureScript = creatureObj->GetComponent<CreatureScript>();

			// ���� ���ط�
			float totalDamage = 5.0f;

			// �ǰ� ȣ��
			creatureScript->OnDamaged(totalDamage);
		}
	}
}
