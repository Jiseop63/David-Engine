#include "daBansheeCombatScript.h"

#include "daGameObject.h"
#include "daCreatureScript.h"
#include "daBansheeProjectileScript.h"
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
		//mCombatTransform->SetScale(math::Vector3(2.50f, 2.50f, 1.0f));
		
		//mEffectScale = math::Vector3(0.70f, 0.70f, 1.0f);
		//mProjectileSize = math::Vector2(0.130f * 4.0f, 0.160f * 4.0f);

		mWeaponInfo = new structs::sWeaponInfo();
		mWeaponInfo->IsAnimationType = false;
		mWeaponInfo->ProjectileStat.EffectName = L"BansheeBulletIdle";
		mWeaponInfo->ProjectileStat.ProjectileType = enums::eProjectileType::Range;
		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.450f;
		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.0f;
		mWeaponInfo->ProjectileStat.ProjectileRange = 4.5f;		
		mWeaponInfo->ProjectileStat.ProjectileSpeed = 2.0f;
	}
	void BansheeCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
	}

	void BansheeCombatScript::ToDoAttack()
	{
		for (int index = 0; index < 12; ++index)
		{
			ActionUnitScript* projectile = mOwnerScript->callActionUnit();
			structs::sActionUnitInfo unitInfo = {};
			unitInfo.Scale = 1.20f;
			unitInfo.Time.End = 2.0f;
			unitInfo.Range = mWeaponInfo->ProjectileStat.ProjectileRange;
			unitInfo.Speed = mWeaponInfo->ProjectileStat.ProjectileSpeed;
			math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, index * 0.520f);
			projectile->SetUnitInfo(unitInfo);
			projectile->SetUnitTypes(UnitRenderType::UsingDirection, UnitUsageType::Default, UnitActionType::Range);
			projectile->SetNextAnimation(L"BansheeBulletIdle", true);
			projectile->SetOffsetPosition(math::Vector3(0.0f, -0.20f, 0.0f));
			projectile->SetMoveDirection(moveDirection);
			projectile->OnActive();
		}
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
