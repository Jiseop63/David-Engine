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

	}
	void BansheeCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
	}

	void BansheeCombatScript::ToDoAttack()
	{
		/*for (int index = 0; index < 12; ++index)
		{
			ActionUnitScript* projectile = mOwnerScript->callActionUnit();
			structs::sActionUnitInfo unitInfo = {};
			unitInfo.Scale = 1.20f;
			unitInfo.DurationTime.End = 2.0f;
			unitInfo.Range = mWeaponInfo->ProjectileStat.ProjectileRange;
			unitInfo.Speed = mWeaponInfo->ProjectileStat.ProjectileSpeed;
			math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, index * 0.520f);
			projectile->SetUnitInfo(unitInfo);
			projectile->SetUnitTypes(enums::eUnitRenderType::UsingDirection, enums::eUnitUsageType::Default, enums::eUnitActionType::Range);
			projectile->SetUnitAnimation(L"BansheeBulletIdle", true);
			projectile->SetUnitOffset(math::Vector3(0.0f, -0.20f, 0.0f));
			projectile->SetUnitDirection(moveDirection);
			projectile->OnActive();
		}*/
	}
	void BansheeCombatScript::OnCollisionEnter(Collider2D* other)
	{
		/*if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			CreatureScript* playerScript = other->GetOwner()->GetComponent<CreatureScript>();
			playerScript->OnDamaged(4.0f);
		}*/
	}
}
