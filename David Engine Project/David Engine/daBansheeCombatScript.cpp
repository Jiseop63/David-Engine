#include "daBansheeCombatScript.h"

#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daProjectileScript.h"
#include "daEffectScript.h"

namespace da
{
	BansheeCombatScript::BansheeCombatScript()
		: mAttackFinished(false)
		, mObjectIndex(0)
	{
	}
	BansheeCombatScript::~BansheeCombatScript()
	{
	}
	void BansheeCombatScript::Initialize()
	{
		CombatScript::Initialize();
		mCombatTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		
		mEffectScale = math::Vector3(0.130f * 4.0f, 0.160f * 4.0f, 1.0f);
		mProjectileSize = math::Vector2(0.130f * 4.0f, 0.160f * 4.0f);

		mWeaponInfo = new structs::sWeaponInfo();

		mWeaponInfo->IsAnimationType = false;
		mWeaponInfo->EffectName = L"BansheeBulletIdle";
		mWeaponInfo->AttackStat.AttackDelayTime = 0.750f;
		mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
		mWeaponInfo->AttackStat.AtaackDamage = 3.0f;
		mWeaponInfo->ProjectileStat.ProjectileType = enums::eProjectileType::Range;
		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.0f;
		mWeaponInfo->ProjectileStat.ProjectileRange = 4.5f;		
		mWeaponInfo->ProjectileStat.ProjectileSpeed = 2.0f;
	}
	void BansheeCombatScript::Update()
	{

	}
	void BansheeCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
	}
	void BansheeCombatScript::AddProjectileObject(GameObject* object)
	{
		ProjectileScript* weaponProjectile = object->AddComponent<ProjectileScript>();
		weaponProjectile->SetCombatScript(this);
		mCombatProjectiles.push_back(weaponProjectile);
	}
	void BansheeCombatScript::ToDoAttack()
	{
		for (mObjectIndex = 0; mObjectIndex < 12; ++mObjectIndex)
		{
			attackEffect();
			attackProjectile();
		}
		
	}
	void BansheeCombatScript::attackEffect()
	{
		// 유효한 객체 가져오기
		EffectScript* effect = callEffect();

		float rotate = 0;
		mObjectIndex;

		// 세팅 해주기
		effect->SetEffectScale(mEffectScale);
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		effect->SetEffectPosition(mCombatTransform->GetPosition());
		effect->SetEffectMoveType(EffectScript::eEffectMoveType::Direction);
		effect->SetProjectileInfo(&mWeaponInfo->ProjectileStat);
		effect->OnActive();

		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mWeaponInfo->EffectName);
	}
	void BansheeCombatScript::attackProjectile()
	{
		// 유효한 객체 가져오기
		ProjectileScript* projectile = callProjectile();

		// 초기화 데이터 세팅
		mWeaponInfo->ProjectileStat.ProjectileActive = true;
		mWeaponInfo->ProjectileStat.ProjectileValidAccumulateTime = 0.0f;
		
		projectile->SetProjectileInfo(&mWeaponInfo->ProjectileStat);
		projectile->SetProjectileSize(mProjectileSize);
		projectile->OnActive();
	}
	void BansheeCombatScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			CreatureScript* playerScript = other->GetOwner()->GetComponent<CreatureScript>();
			playerScript->OnDamaged(4.0f);
		}
	}
}
