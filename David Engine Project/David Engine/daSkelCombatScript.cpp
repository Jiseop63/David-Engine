#include "daSkelCombatScript.h"

#include "daTime.h"
#include "daInput.h"
#include "daResources.h"
#include "daGameObject.h"

#include "daAnimator.h"
#include "daMeshRenderer.h"

#include "daMonsterScript.h"
#include "daActionUnitScript.h"


namespace da
{
	SkelCombatScript::SkelCombatScript()
		: mAttackFinished(false)
	{
	}
	SkelCombatScript::~SkelCombatScript()
	{
	}
	void SkelCombatScript::Initialize()
	{
		CombatScript::Initialize();

		// 애니메이션 & 이벤트		
		mCombatAnimator->Create(L"EnemyGreatSwordIdle", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 1, math::Vector2::Zero, 0.0f, 70.0f);
				
		mCombatAnimator->Create(L"EnemyGreatSwordSwing", Resources::Find<Texture>(L"EnemyGreatSword")
			, math::Vector2::Zero, math::Vector2(51.0f, 49.0f), 16, math::Vector2::Zero, 0.050f, 70.0f);
		mCombatAnimator->ActionEvent(L"EnemyGreatSwordSwing", 7) = std::bind(&SkelCombatScript::attackProjectile, this);
		mCombatAnimator->CompleteEvent(L"EnemyGreatSwordSwing") = std::bind(&SkelCombatScript::attackFinished, this);
		mCombatAnimator->PlayAnimation(L"EnemyGreatSwordIdle", false);


		mProjectileSize = math::Vector2(1.20f, 1.40f);
		mCombatTransform->SetScale(math::Vector3(0.510f * 4.0f, 0.490f * 4.0f, 1.0f));

		mWeaponInfo = new structs::sWeaponInfo();
		mWeaponInfo->IsAnimationType = true;
		mWeaponInfo->AnimationName = L"EnemyGreatSwordSwing";
		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.30f;
		mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
		mWeaponInfo->ProjectileStat.ProjectileAngle = -1.570f;

	}
	void SkelCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
		CombatScript::updateReverseRenderer();

		bool value = isLeft();
		if (value)
			mWeaponInfo->ProjectileStat.ProjectileAngle = 1.570f;
		else
			mWeaponInfo->ProjectileStat.ProjectileAngle = -1.570f;

	}
	void SkelCombatScript::ToDoAttack()
	{
		CombatScript::attackPlay();		
	}

	void SkelCombatScript::attackProjectile()
	{
		ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();
		structs::sActionUnitInfo unitInfo = {};
		unitInfo.Scale = 1.20f;
		unitInfo.DurationTime.End = 0.20f;
		unitInfo.RotateAngle = mWeaponInfo->ProjectileStat.ProjectileAngle;

		actionUnit->SetUnitTypes(enums::eUnitUsageType::OnlyCollider, enums::eUnitRenderType::JustRotate, enums::eUnitActionType::Duration);
		actionUnit->SetUnitInfo(unitInfo);
		actionUnit->SetOffsetPosition(math::Vector3(mOwnerDir.x * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, mOwnerDir.y * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, 0.0f)); // 아직 문제있음

		actionUnit->OnActive();
	}

	void SkelCombatScript::OnCollisionEnter(Collider2D* other)
	{
		if (enums::eLayerType::Playable == other->GetOwner()->GetLayerType()
			&& other->IsBody())
		{
			CreatureScript* playerScript = other->GetOwner()->GetComponent<CreatureScript>();
			playerScript->OnDamaged(4.0f);			
		}		
	}
}
