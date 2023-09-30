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
		// �ʱ�ȭ �� ���� ��ü ������ֱ�
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
		// # �� ��ġ ��������
		math::Vector3 myPosition = mCombatTransform->GetPosition();

		// ���� ȸ������ ���ϱ�
		float angle = atan2(mOwnerDir.y, mOwnerDir.x);
		mEffectAngle = angle;

		// Weapon Position ���ϱ�
		math::Vector2 armUp(-0.050f, 0.0f);
		math::Vector2 armDown(0.050f, -0.30f);

		// �����Ҷ����� ���� ȸ������ ��ġ�� ����
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
		// ����� ���� Weapon Transform�� �����ϱ�
		mWeaponInfo->ProjectileStat.ProjectileAngle = mEffectAngle - 1.570f;
		mCombatTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mCombatTransform->SetPosition(myPosition);
	}
	void PlayerCombatScript::EquipWeapon()
	{
		// ���߿��� ���⼭ �����ϴ°� �ƴ϶� �������� ���õǾ �Ѱ��� ������
		switch (mWeaponInfo->WeaponName)
		{
		case enums::eWeaponName::Default:
		{
			// ���� ���� ����
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
			// ���� ���� ����
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
			CombatScript::attackPlay();		// �̰� ����ϴ°� ����
			ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();
			structs::sActionUnitInfo unitInfo = {};
			unitInfo.Scale = 1.60f;
			unitInfo.DurationTime.End = 0.250f;
			unitInfo.RotateAngle = mEffectAngle - 1.570f;
			
			actionUnit->SetUnitTypes(enums::eUnitUsageType::Default, enums::eUnitRenderType::JustRotate, enums::eUnitActionType::None);
			actionUnit->SetUnitInfo(unitInfo);
			actionUnit->SetNextAnimation(mWeaponInfo->ProjectileStat.EffectName, false);
			actionUnit->SetOffsetPosition(math::Vector3(mOwnerDir.x * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, mOwnerDir.y * mWeaponInfo->ProjectileStat.ProjectileCenterPadding, 0.0f)); // ���� ��������
			actionUnit->OnActive();

			mWeaponInfo->AttackStat.AttackReady = false;
		}
	}

}