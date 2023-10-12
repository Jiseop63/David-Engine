#include "daSkelCombatScript.h"

#include "daTime.h"
#include "daInput.h"
#include "daResources.h"
#include "daGameObject.h"

#include "daAnimator.h"
#include "daMeshRenderer.h"

#include "daMonsterScript.h"
#include "daActionUnitScript.h"
#include "daAudioSource.h"

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


		mCombatTransform->SetScale(math::Vector3(0.510f * 4.0f, 0.490f * 4.0f, 1.0f));

		// mProjectileSize = math::Vector2(1.20f, 1.40f);
		// mWeaponInfo = new structs::sWeaponInfo();
		// mWeaponInfo->IsAnimationType = true;
		// mWeaponInfo->AnimationName = L"EnemyGreatSwordSwing";
		// mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
		// mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.30f;
		// mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
		// mWeaponInfo->ProjectileStat.ProjectileAngle = -1.570f;
	}
	void SkelCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
		CombatScript::updateReverseRenderer();
		bool value = isLeft();
		if (value)
			mWeaponAngle = 1.570f;
		else
			mWeaponAngle = -1.570f;

	}
	void SkelCombatScript::ToDoAttack()
	{
		mCombatAnimator->PlayAnimation(L"EnemyGreatSwordSwing", false);
	}

	void SkelCombatScript::attackProjectile()
	{
		ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();
		actionUnit->SetUnitPosition(mOwnerPosition);

		structs::sActionUnitTypes mMonsterUnitTypes = {};
		mMonsterUnitTypes.Usage = enums::eUnitUsageType::InvisibleProjectile;
		mMonsterUnitTypes.LifeCycle = enums::eUnitLifeType::Duration;
		mMonsterUnitTypes.Action = enums::eUnitActionType::UsingRotation;
		actionUnit->SetUnitTypes(mMonsterUnitTypes);

		structs::sActionUnitStat mMonsterUnitInfo = {};
		mMonsterUnitInfo.Speed = 0.01f;
		mMonsterUnitInfo.DurationTime.End = 0.250f;
		mMonsterUnitInfo.AtaackDamage = 2.0f;
		actionUnit->SetUnitInfo(mMonsterUnitInfo);
		actionUnit->SetUnitScale(math::Vector3(1.20f, 1.40f, 1.0f));
		actionUnit->SetUnitRotateAngle(mWeaponAngle);

		math::Vector3 offsetPosition = math::Vector3::Zero;
		offsetPosition.x = mOwnerDir.x * 0.30f;
		offsetPosition.y = mOwnerDir.y * 0.30f;
		actionUnit->SetUnitOffset(offsetPosition);

		actionUnit->OnActive();
		mOwnerScript->GetCreatureAudio()->Play(Resources::Find<AudioClip>(L"swish"), 60.0f);		
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
