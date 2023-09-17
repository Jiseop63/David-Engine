#include "daPlayerCombatScript.h"

#include "daGameDataManager.h"
#include "daSceneManager.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"
#include "daAttackEffectScript.h"
#include "daProjectileScript.h"
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
		// mActiveArmour = GameDataManager::GetActiveArmour();
		// = SceneManager::GetPlayerScript();

		mWeaponInfo = new structs::sWeaponInfo();
		EquipWeapon();
		int a = 0;
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
		
#pragma region Update & lateUpdate Func

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
	void PlayerCombatScript::attackProjectile()
	{
		// ��ȿ�� ��ü ��������
		ProjectileScript* projectile = callProjectile();

		// �ʱ�ȭ ������ ����
		mWeaponInfo->ProjectileStat.ProjectileActive = true;
		mWeaponInfo->ProjectileStat.ProjectileValidAccumulateTime = 0.0f;
		
		projectile->SetProjectileInfo(&mWeaponInfo->ProjectileStat);
		projectile->SetProjectileSize(mProjectileSize);

		projectile->OnActive();
	}
#pragma endregion
#pragma region Initialize & Get Script
	void PlayerCombatScript::AddEffectObject(GameObject* object)
	{
		AttackEffectScript* weaponEffect = object->AddComponent<AttackEffectScript>();
		weaponEffect->SetReqWeapon(this);
		mCombatEffects.push_back(weaponEffect);
	}
	void PlayerCombatScript::AddProjectileObject(GameObject* object)
	{
		ProjectileScript* weaponProjectile = object->AddComponent<ProjectileScript>();
		weaponProjectile->SetCombatScript(this);
		mCombatProjectiles.push_back(weaponProjectile);
	}
#pragma endregion
#pragma region Player call
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
			mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.750f;
			mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
			mProjectileSize = math::Vector2(2.50f, 1.750f);

			mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mWeaponInfo->EffectName = L"GreatSwing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mCombatRenderer->ChangeSlotTexture(mWeaponTexture);
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
			mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.750f;
			mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
			mProjectileSize = math::Vector2(2.50f, 1.750f);
			mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mWeaponInfo->EffectName = L"GreatSwing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mCombatRenderer->ChangeSlotTexture(mWeaponTexture);
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
			CombatScript::attackPlay();
			CombatScript::attackEffect();
			attackProjectile();
			mWeaponInfo->AttackStat.AttackReady = false;
		}
	}
	
#pragma endregion
}