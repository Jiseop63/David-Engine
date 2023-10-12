#include "daPlayerCombatScript.h"

#include "daGameDataManager.h"
#include "daSceneManager.h"
#include "daInput.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"

#include "daActionUnitScript.h"
#include "daPlayerScript.h"

namespace da
{
	PlayerCombatScript::PlayerCombatScript()
		: mWeaponItem(nullptr)
		, mWeaponTexture(nullptr)
		, mWeaponCooldownReady(true)
		, mWeaponCooldownTime{}
	{
	}
	PlayerCombatScript::~PlayerCombatScript()
	{
	}

	void PlayerCombatScript::Initialize()
	{
		CombatScript::Initialize();
		mCombatAnimator->Create(L"CrossbowIdle", Resources::Find<Texture>(L"CrossbowIdle"), math::Vector2::Zero, math::Vector2(18.0f, 11.0f), 1, math::Vector2::Zero, 0.050f, 25.0f);
		mCombatAnimator->Create(L"CrossbowShot", Resources::Find<Texture>(L"CrossbowShot"), math::Vector2::Zero, math::Vector2(18.0f, 11.0f), 2, math::Vector2::Zero, 0.050f, 25.0f);
		mCombatAnimator->CompleteEvent(L"CrossbowShot") = std::bind(&PlayerCombatScript::retCrossbow, this);
	}
	void PlayerCombatScript::Update()
	{
		if (mWeaponItem)
		{
			if (mWeaponItem->GetItemInfo().UseAnimation)
			{
				if (Input::GetKey(eKeyCode::LBTN))
					ToDoAttack();
			}
			else
			{
				if (Input::GetKeyDown(eKeyCode::LBTN))
					ToDoAttack();
			}

			if (enums::eItemAttackType::Swing == mWeaponItem->GetItemInfo().AttackType)
			{
				if (Input::GetKeyDown(eKeyCode::LBTN))
					ToDoAttack();
			}
			else if (enums::eItemAttackType::Shoot == mWeaponItem->GetItemInfo().AttackType)
			{
				if (Input::GetKey(eKeyCode::LBTN))
					ToDoAttack();
			}
			else if (enums::eItemAttackType::Spin == mWeaponItem->GetItemInfo().AttackType)
			{
				if (Input::GetKeyDown(eKeyCode::LBTN))
					mIsSpin = true;
				if (Input::GetKeyUp(eKeyCode::LBTN))
					mIsSpin = false;

				if (Input::GetKey(eKeyCode::LBTN))
					ToDoAttack();
			}

		}				
		updateAttackCooldown();
	}
	void PlayerCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
		CombatScript::updateReverseRenderer();
		updateWeaponRotation();

		if (mWeaponItem)
		{
			if (enums::eItemAttackType::Spin == mWeaponItem->GetItemInfo().AttackType)
			{
				if (mIsSpin)
				{
					float currrentZ = mCombatTransform->GetRotation().z;
					mSpinRotate += (float)Time::DeltaTime() * 30.0f;
					currrentZ += mSpinRotate;
					mCombatTransform->SetRotation(math::Vector3(0.0f,0.0f, currrentZ));
				}
				else
				{
					mSpinRotate = 0.0f;
					float rotateZ = mCombatTransform->GetRotation().z;
					rotateZ += 0.7850f;
					mCombatTransform->SetRotation(math::Vector3(0.0f, 0.0f, rotateZ));
				}
			}
		}

		
	}
	void PlayerCombatScript::EquipWeapon(ItemScript* weaponItem)
	{
		// 초기화
		if (mWeaponItem)
			mWeaponItem->GetItemInfo().AttackDelayTime.Clear();

		mWeaponItem = weaponItem;

		// 아이템 공격 세팅
		if (weaponItem)
		{
			mWeaponCooldownReady = true;
			mWeaponCooldownTime = mWeaponItem->GetItemInfo().AttackDelayTime;
		}
		
		// 아이템 렌더링 세팅
		if (!mWeaponItem)
		{
			mCombatRenderer->ChangeMaterialTexture(nullptr);
		}
		else
		{
			mCombatRenderer->SetMaterial(mWeaponItem->GetItemMaterial());
			if (mWeaponItem->GetItemInfo().UseAnimation)
			{
				mCombatTransform->SetScale(math::Vector3::One);
				mCombatAnimator->ApplyComponentUsing(true);
				mCombatAnimator->PlayAnimation(mWeaponItem->GetItemInfo().Animation.Idle);
			}
			else
			{
				mCombatTransform->SetScale(mWeaponItem->GetItemScale());
				mCombatRenderer->ChangeMaterialTexture(mWeaponItem->GetItemTexture());
				mCombatAnimator->ApplyComponentUsing(false);
			}
		}
	}

	void PlayerCombatScript::updateWeaponRotation()
	{
		if (!mWeaponItem)
			return;
		// 오프셋 적용하기
		math::Vector3 myPosition = mCombatTransform->GetPosition();
		math::Vector3 weaponOffset = mWeaponItem->GetItemOffset();
		if (isLeft())
			weaponOffset.x *= -1;




		// 무기 회전방향 구하기
		mOwnerDir = mOwnerScript->GetCreatureDir();
		float angle = atan2(mOwnerDir.y, mOwnerDir.x);
		mWeaponAngle = angle;


		if (enums::eItemAttackType::Swing == mWeaponItem->GetItemInfo().AttackType)
		{
			// 공격할때마다 무기 회전값과 위치값 적용
			if (isLeft())
			{
				if (mWeaponAttacked)
				{
					angle += 2.710f;
					myPosition.x -= weaponOffset.x;
					myPosition.y += weaponOffset.y * 1.40f;
				}
				else
				{
					angle -= 0.3930f;
					myPosition.x += weaponOffset.x;
					myPosition.y += weaponOffset.y;
				}
			}
			else
			{
				if (mWeaponAttacked)
				{
					angle -= 2.710f;
					myPosition.x -= weaponOffset.x;
					myPosition.y += weaponOffset.y * 1.40f;
				}
				else
				{
					angle += 0.3930f;
					myPosition.x += weaponOffset.x;
					myPosition.y += weaponOffset.y;
				}
			}

			myPosition.x -= mOwnerDir.x * 0.050f;
			myPosition.y -= mOwnerDir.y * 0.050f + weaponOffset.y;
			mCombatTransform->SetPosition(myPosition);
			// 변경된 값을 Weapon Transform에 적용하기
		}
		else if (enums::eItemAttackType::Shoot == mWeaponItem->GetItemInfo().AttackType)
		{
			mCombatTransform->SetPosition(myPosition + weaponOffset);
		}

		mCombatTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
	}

	void PlayerCombatScript::updateAttackCooldown()
	{
		if (!mWeaponCooldownReady)
		{
			mWeaponCooldownTime.Start += (float)Time::DeltaTime();

			if (mWeaponCooldownTime.End <= mWeaponCooldownTime.Start)
			{
				mWeaponCooldownReady = true;
				mWeaponCooldownTime.Start = 0.0f;
			}
		}
	}
	void PlayerCombatScript::retCrossbow()
	{
		mCombatAnimator->PlayAnimation(L"CrossbowIdle", true);
	}
	void PlayerCombatScript::ToDoAttack()
	{
		if (!mWeaponItem)
		{
			ItemScript* activeItem = dynamic_cast<PlayerScript*>(mOwnerScript)->GetInventoryScript()->GetActiveItemScript();
			if (activeItem)
				EquipWeapon(activeItem);
			else
				return;
		}

		if (mWeaponCooldownReady)
		{
			if (enums::eItemAttackType::Swing == mWeaponItem->GetItemInfo().AttackType)
			{
				if (mWeaponAttacked)
					mWeaponAttacked = false;
				else
					mWeaponAttacked = true;
			}
			else if (enums::eItemAttackType::Shoot == mWeaponItem->GetItemInfo().AttackType)
			{
				mCombatAnimator->PlayAnimation(mWeaponItem->GetItemInfo().Animation.Action);
			}
			

			ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();

			actionUnit->SetUnitTypes(mWeaponItem->GetProjectileTypes());
			actionUnit->SetUnitInfo(mWeaponItem->GetProjectileStat());

			actionUnit->SetUnitRotateAngle(mWeaponAngle - 1.570f);
			math::Vector3 offsetPosition = mWeaponItem->GetProjectileOffset();
			offsetPosition.x *= mOwnerDir.x;
			offsetPosition.y *= mOwnerDir.y;
			actionUnit->SetUnitScale(mWeaponItem->GetProjectileScale());
			actionUnit->SetUnitOffset(offsetPosition);
			actionUnit->SetUnitColliderSize(mWeaponItem->GetProjectileSize());

			actionUnit->OnActive();

			mWeaponCooldownReady = false;
			mOwnerScript->GetCreatureAudio()->Play(Resources::Find<AudioClip>(mWeaponItem->GetItemInfo().Sound), 60.0f);
		}
	}

}