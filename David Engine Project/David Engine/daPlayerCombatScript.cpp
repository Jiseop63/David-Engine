#include "daPlayerCombatScript.h"

#include "daGameDataManager.h"
#include "daSceneManager.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"

#include "daActionUnitScript.h"
#include "daPlayerScript.h"

namespace da
{
	PlayerCombatScript::PlayerCombatScript()
		: mWeaponTexture(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
	{		
	}
	PlayerCombatScript::~PlayerCombatScript()
	{
	}

	void PlayerCombatScript::Initialize()
	{
		CombatScript::Initialize();

		mWeaponInfo = new structs::sWeaponInfo();
		EquipWeapon();
		// 초기화 및 관련 객체 등록해주기
	}
	void PlayerCombatScript::Update()
	{		
		CombatScript::updateAttackCoolDown();
	}
	void PlayerCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
		CombatScript::updateReverseRenderer();
		updateWeaponRotate();
	}
		
	void PlayerCombatScript::updateWeaponRotate()
	{
		// # 내 위치 가져오기
		math::Vector3 myPosition = mCombatTransform->GetPosition();

		// 무기 회전방향 구하기
		float angle = atan2(mOwnerDir.y, mOwnerDir.x);
		mEffectAngle = angle;

		// Weapon Position 구하기
		math::Vector2 armUp(-0.050f, 0.0f);
		math::Vector2 armDown(0.050f, -0.30f);

		// 공격할때마다 무기 회전값과 위치값 적용
		if (isLeft())
		{
			if (!mWeaponAttacked)
			{
				angle += 2.710f;
				myPosition.x += armUp.x;
				myPosition.y += armDown.y;
			}
			else
			{
				angle -= 0.3930f;
				myPosition.x += armDown.x;
			}
		}
		else
		{
			if (!mWeaponAttacked)
			{
				angle -= 2.710f;
				myPosition.x -= armUp.x;
				myPosition.y += armDown.y;

			}
			else
			{
				angle += 0.3930f;
				myPosition.x -= armDown.x;
			}
		}

		myPosition.x -= mOwnerDir.x * 0.050f;
		myPosition.y -= mOwnerDir.y * 0.050f;
		// 변경된 값을 Weapon Transform에 적용하기
		mWeaponInfo->ProjectileStat.ProjectileAngle = mEffectAngle - 1.570f;
		mCombatTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mCombatTransform->SetPosition(myPosition);
	}
	void PlayerCombatScript::EquipWeapon()
	{
		// 나중에는 여기서 세팅하는게 아니라 아이템이 세팅되어서 넘겨질 예정임
		switch (mWeaponInfo->WeaponName)
		{
		case enums::eWeaponName::Default:
		{
			// 무기 정보 세팅
			mWeaponInfo->IsAnimationType = false;
			mWeaponInfo->AttackStat.AttackDelayTime = 0.450f;
			mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
			mWeaponInfo->AttackStat.AtaackDamage = 5.0f;
			
			mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
			mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.50f;
			mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
			mProjectileSize = math::Vector2(2.50f, 1.750f);

			mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mWeaponInfo->ProjectileStat.EffectName = L"Swing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mCombatRenderer->ChangeMaterialTexture(mWeaponTexture);

			GameObject::eObjectState test = GetOwner()->GetObjectState();

			int a = 0;
		}
		break;
		case enums::eWeaponName::LongSword:
		{
			// 무기 정보 세팅
			mWeaponInfo->IsAnimationType = false;
			mWeaponInfo->AttackStat.AttackDelayTime = 0.450f;
			mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
			mWeaponInfo->AttackStat.AtaackDamage = 5.0f;
			mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
			mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.50f;
			mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
			mProjectileSize = math::Vector2(2.50f, 1.750f);
			mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mWeaponInfo->ProjectileStat.EffectName = L"Swing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mCombatRenderer->ChangeMaterialTexture(mWeaponTexture);
		}
		break;
		default:
			break;
		}
	}
	void PlayerCombatScript::ToDoAttack()
	{
		if (mWeaponInfo->AttackStat.AttackReady)
		{
			CombatScript::attackPlay();		// 이건 사용하는게 맞음
			ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();
			structs::sActionUnitInfo unitInfo = {};
			unitInfo.Scale = 1.60f;
			unitInfo.DurationTime.End = 0.250f;
			unitInfo.RotateAngle = mEffectAngle - 1.570f;
			
			actionUnit->SetUnitTypes(enums::eUnitUsageType::Default, enums::eUnitRenderType::JustRotate, enums::eUnitActionType::None);
			actionUnit->SetUnitInfo(unitInfo);
			actionUnit->SetNextAnimation(mWeaponInfo->ProjectileStat.EffectName, false);
			actionUnit->SetOffsetPosition(math::Vector3(mOwnerDir.x * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, mOwnerDir.y * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, 0.0f)); // 아직 문제있음
			actionUnit->OnActive();

			mWeaponInfo->AttackStat.AttackReady = false;
		}
	}

}