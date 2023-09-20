#include "daBansheeCombatScript.h"
#include "daBansheeCombatScript.h"

#include "daGameObject.h"
#include "daCreatureScript.h"
#include "dalProjectileScript.h"
#include "daBansheeAttackEffect.h"

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
		
		//mEffectScale = math::Vector3(0.70f, 0.70f, 1.0f);
		mProjectileSize = math::Vector2(0.130f * 4.0f, 0.160f * 4.0f);

		mWeaponInfo = new structs::sWeaponInfo();

		mWeaponInfo->IsAnimationType = false;
		mWeaponInfo->ProjectileStat.EffectName = L"BansheeBulletIdle";
		mWeaponInfo->ProjectileStat.ProjectileType = enums::eProjectileType::Range;
		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.0f;
		mWeaponInfo->ProjectileStat.ProjectileRange = 5.5f;		
		mWeaponInfo->ProjectileStat.ProjectileSpeed = 2.0f;
	}
	void BansheeCombatScript::LateUpdate()
	{
		updateWeaponPosition();
	}
	void BansheeCombatScript::AddEffectObject(GameObject* object)
	{
		BansheeAttackEffectScript* weaponProjectile = object->AddComponent<BansheeAttackEffectScript>();
		weaponProjectile->SetCombatScript(this);
		mCombatEffects.push_back(weaponProjectile);
	}
	void BansheeCombatScript::AddProjectileObject(GameObject* object)
	{
		lProjectileScript* weaponProjectile = object->AddComponent<lProjectileScript>();
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
		mObjectIndex = 0;
	}
	void BansheeCombatScript::attackEffect()
	{
		// 유효한 객체 가져오기
		EffectScript* effect = callEffect();
		math::Vector3 effectDir = math::daRotateVector3(math::Vector3::Up, mObjectIndex * 0.520f);

		BansheeAttackEffectScript* bansheeEffect = dynamic_cast<BansheeAttackEffectScript*>(effect);

		// 세팅 해주기
		//effect->SetEffectScale(mEffectScale);
		bansheeEffect->SetEffectPosition(mCombatTransform->GetPosition());
		bansheeEffect->SetEffectMoveType(EffectScript::eEffectMoveType::Direction);
		bansheeEffect->SetEffectDir(effectDir);
		bansheeEffect->SetProjectileInfo(&mWeaponInfo->ProjectileStat);
		bansheeEffect->OnActive();
	}
	void BansheeCombatScript::attackProjectile()
	{
		// 유효한 객체 가져오기
		lProjectileScript* projectile = callProjectile();

		// 초기화 데이터 세팅
		mWeaponInfo->ProjectileStat.ProjectileActive = true;
		mWeaponInfo->ProjectileStat.ProjectileValidAccumulateTime = 0.0f;
		mWeaponInfo->ProjectileStat.ProjectileAngle = mObjectIndex * 0.520f;
		projectile->SetProjectileInfo(&mWeaponInfo->ProjectileStat);
		projectile->SetProjectileSize(mProjectileSize);
		projectile->OnActive();
	}
	void BansheeCombatScript::updateWeaponPosition()
	{
		// 플레이어 위치 가져오기
		math::Vector3 ownerPosition = mOwnerScript->GetCreatureTransform()->GetPosition();
		// 내 위치에 적용하기
		mCombatTransform->SetPosition(ownerPosition);
		mOwnerDir = mOwnerScript->GetCreatureDir();
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
