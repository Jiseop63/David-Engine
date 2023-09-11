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
	CombatScript::CombatScript()
		: mWeaponTransform(nullptr)
		, mWeaponRenderer(nullptr)
		, mWeaponAnimator(nullptr)
		, mPlayerScript(nullptr)
		, mEffects{}
		, mProjectiles{}
		, mActiveArmour(nullptr)
		, mWeaponTexture(nullptr)
		, mFirstTexture(nullptr)
		, mSecondTexture(nullptr)

		, mEffectScale(math::Vector3::One)
		, mProjectileScale(math::Vector2::One)

		, mPlayerDir(math::Vector2::Zero)
		, mHitEffectAngle(0.0f)
		, mWeaponAttacked(false)
		, mEffectAngle(0.0f)
	{		
	}
	CombatScript::~CombatScript()
	{
	}

	void CombatScript::Initialize()
	{
		// Tr세팅
		mWeaponTransform = GetOwner()->GetTransform();
		mWeaponTransform->SetScale(math::Vector3(2.0f, 2.0f, 1.0f));

		// 근거리 무기는 Texture, 원거리 무기는 Animation 을 사용함
		mWeaponRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mWeaponAnimator = GetOwner()->AddComponent<Animator>();

		// 데이터 연동
		mActiveArmour = GameDataManager::GetActiveArmour();
		mPlayerScript = SceneManager::GetPlayerScript();
		EquipWeapon();

		// 무기 세팅
		//ChangeWeapon();
	}
	void CombatScript::Update()
	{		
		updateAttackCoolDown();
	}
	void CombatScript::LateUpdate()
	{
		updateWeaponPosition();
		updateWeaponRotate();
		updateReverseRenderer();
	}
		
#pragma region Update & lateUpdate Func

	void CombatScript::updateWeaponPosition()
	{
		// 플레이어 위치 가져오기
		math::Vector3 playerPosition = mPlayerScript->GetCreatureTransform()->GetPosition();
		// 내 위치에 적용하기
		mWeaponTransform->SetPosition(playerPosition);
	}
	void CombatScript::updateWeaponRotate()
	{
		// 회전값 적용하기 + 방향 적용하기


		// # 플레이어 방향 가져오기
		mPlayerDir = mPlayerScript->GetCreatureDir();
		// # 내 위치 가져오기
		math::Vector3 myPosition = mWeaponTransform->GetPosition();

		// 무기 회전방향 구하기
		float angle = atan2(mPlayerDir.y, mPlayerDir.x);
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

		myPosition.x -= mPlayerDir.x * 0.050f;
		myPosition.y -= mPlayerDir.y * 0.050f;
		// 변경된 값을 Weapon Transform에 적용하기
		mWeaponTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mWeaponTransform->SetPosition(myPosition);
	}
	void CombatScript::updateAttackCoolDown()
	{
		// 쿨다운 적용하기
		if (!mActiveArmour->Weapon.AttackReady)
		{
			mActiveArmour->Weapon.AttackDelayAccumulateTime += (float)Time::DeltaTime();

			if (mActiveArmour->Weapon.AttackDelayTime <= mActiveArmour->Weapon.AttackDelayAccumulateTime)
			{
				mActiveArmour->Weapon.AttackReady = true;
				mActiveArmour->Weapon.AttackDelayAccumulateTime = 0.0f;
			}
		}
	}

#pragma endregion
#pragma region Initialize & Get Script
	void CombatScript::AddEffectObject(GameObject* object)
	{
		AttackEffectScript* weaponEffect = object->AddComponent<AttackEffectScript>();
		weaponEffect->SetReqWeapon(this);
		mEffects.push_back(weaponEffect);
	}
	void CombatScript::AddProjectileObject(GameObject* object)
	{
		ProjectileScript* weaponProjectile = object->AddComponent<ProjectileScript>();
		weaponProjectile->SetReqWeapon(this);
		mProjectiles.push_back(weaponProjectile);
	}
	EffectScript* CombatScript::callEffect()
	{
		for (size_t effect = 0; effect < mEffects.size(); effect++)
		{
			if (GameObject::eObjectState::Inactive ==
				mEffects[effect]->GetOwner()->GetObjectState())
				return mEffects[effect];
		}
		return nullptr;
	}
	ProjectileScript* CombatScript::callProjectile()
	{
		for (size_t projectile = 0; projectile < mProjectiles.size(); ++projectile)
		{
			if (GameObject::eObjectState::Inactive ==
				mProjectiles[projectile]->GetOwner()->GetObjectState())
				return mProjectiles[projectile];
		}
		return nullptr;
	}
#pragma endregion
#pragma region Player call
	void CombatScript::EquipWeapon()
	{
		switch (mActiveArmour->Weapon.WeaponName)
		{
		case enums::eWeaponName::Default:
		{
			// 무기 정보 세팅
			mActiveArmour->Weapon.AttackDelayTime = 0.450f;
			mActiveArmour->Weapon.AttackDelayAccumulateTime = 0.0f;
			mActiveArmour->Weapon.AtaackDamage = 5.0f;
			mActiveArmour->Weapon.ProjectileValidTime = 0.250f;
			mActiveArmour->Weapon.ProjectileCenterPadding = 0.750f;
			mActiveArmour->Weapon.ProjectileRange = 3.5f;
			mProjectileScale = math::Vector2(2.50f, 1.750f);
			mWeaponTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mEffectName = L"GreatSwing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mWeaponRenderer->ChangeSlotTexture(mWeaponTexture);
		}
		break;
		case enums::eWeaponName::LongSword:
		{
			// 무기 정보 세팅
			mActiveArmour->Weapon.AttackDelayTime = 0.450f;
			mActiveArmour->Weapon.AttackDelayAccumulateTime = 0.0f;
			mActiveArmour->Weapon.AtaackDamage = 5.0f;
			mActiveArmour->Weapon.ProjectileValidTime = 0.250f;
			mActiveArmour->Weapon.ProjectileCenterPadding = 0.750f;
			mActiveArmour->Weapon.ProjectileRange = 3.5f;
			mProjectileScale = math::Vector2(2.50f, 1.750f);
			mWeaponTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
			mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
			mEffectName = L"GreatSwing";
			mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

			if (mWeaponTexture)
				mWeaponRenderer->ChangeSlotTexture(mWeaponTexture);
		}
		break;
		default:
			break;
		}
	}
	void CombatScript::ToDoAttack()
	{
		if (mActiveArmour->Weapon.AttackReady)
		{
			playWeaponAttack();
			mActiveArmour->Weapon.AttackReady = false;
		}
	}
	void CombatScript::CallHitEffect(math::Vector3 position)
	{
		if (7 <= mHitEffectAngle)
			mHitEffectAngle = 0.0f;
		mHitEffectAngle += 1.80f;
		// 방향 구하기
		EffectScript* effect = callEffect();
		effect->SetEffectScale(math::Vector3(1.50f, 1.50f, 1.0f));
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mHitEffectAngle));
		effect->SetEffectPosition(position - math::Vector3(0.0f, 0.2f, 0.0f));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(L"Slash");
	}
#pragma endregion
#pragma region Atttack process
	void CombatScript::playWeaponAttack()
	{
		attackEffect();
		attackProjectile();
		attackAnimation();
	}
	void CombatScript::attackEffect()
	{
		// 유효한 객체 가져오기
		EffectScript* effect = callEffect();
		// 세팅 해주기
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		effect->SetEffectScale(mEffectScale);
		effect->SetEffectRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		effect->SetEffectPosition(mWeaponTransform->GetPosition() + (playerDir * mActiveArmour->Weapon.ProjectileCenterPadding));
		effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		effect->PlayEffect(mEffectName);
	}
	void CombatScript::attackProjectile()
	{
		// 유효한 객체 가져오기
		ProjectileScript* projectile = callProjectile();
		// 세팅 해주기
		math::Vector3 playerDir(mPlayerDir.x, mPlayerDir.y, 0.0f);
		projectile->SetProjectileRotation(math::Vector3(0.0f, 0.0f, mEffectAngle - 1.570f));
		projectile->SetProjectilePosition(mWeaponTransform->GetPosition() + (playerDir * mActiveArmour->Weapon.ProjectileCenterPadding));
		projectile->SetBeginProjectile(mWeaponTransform->GetPosition());
		projectile->SetProjectileCenter(mPlayerDir * mActiveArmour->Weapon.ProjectileCenterPadding);
		projectile->SetProjectileSize(mProjectileScale);
		projectile->SetProjectileType(mActiveArmour->Weapon.ProjectileType);
		projectile->SetWeaponProjectile(mActiveArmour->Weapon);
		projectile->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void CombatScript::attackAnimation()
	{
		if (mActiveArmour->Weapon.IsMeleeWeapon)
		{
			// Weapon Texture
			if (mWeaponAttacked)
				mWeaponAttacked = false;
			else
				mWeaponAttacked = true;
		}
		else
		{
			// Animation Play
		}
	}
#pragma endregion	
}