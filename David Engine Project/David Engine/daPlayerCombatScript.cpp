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
		
#pragma region Update & lateUpdate Func

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
	void PlayerCombatScript::attackProjectile()
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
			// 무기 정보 세팅
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