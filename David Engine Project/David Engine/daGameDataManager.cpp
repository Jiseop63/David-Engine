#include "daGameDataManager.h"

#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"

namespace da
{
	using namespace math;
	structs::sPlayerStat GameDataManager::mPlayerStat = {};
	structs::sJumpCount GameDataManager::mJumpCount = {};
	structs::sDashCount GameDataManager::mDashCount = {};
	structs::sInventory GameDataManager::mInventoryData = {};

	structs::sArmour GameDataManager::mActiveArmour = {};
	structs::sArmour GameDataManager::mSubArmour = {};

	GameObject* GameDataManager::mInventoryObject = nullptr;
	GameObject* GameDataManager::mWeaponObject = nullptr;
	GameObject* GameDataManager::mPlayerObject = nullptr;

	GameObject* GameDataManager::mHPBar = nullptr;
	GameObject* GameDataManager::mDashCountBar = nullptr;
	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 2.0f;
		mPlayerStat.JumpForce = 4.50f;
		mPlayerStat.DashForce = 6.0f;
		mDashCount.MaxDashCount = 2;
		mDashCount.CurDashCount = mDashCount.MaxDashCount;
		mJumpCount.MaxJumpCount = 2;
		mJumpCount.CurJumpCount = mJumpCount.MaxJumpCount;
		
		mInventoryData.Armour1.Weapon = enums::eWeaponType::LongSword;
		mActiveArmour = mInventoryData.Armour1;
		mSubArmour = mInventoryData.Armour2;
	}

	void GameDataManager::GetDamage(float value)
	{
		mPlayerStat.CurHP -= value;
		if (0 >= mPlayerStat.CurHP)
			mPlayerStat.CurHP = 0;
		mHPBar->GetComponent<LifeBarScript>()->BindConstantBuffer();
	}

	void GameDataManager::GetHeal(float value)
	{
		mPlayerStat.CurHP += value;
		if (mPlayerStat.MaxHP <= mPlayerStat.CurHP)
			mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mHPBar->GetComponent<LifeBarScript>()->BindConstantBuffer();
	}

	bool GameDataManager::UseDash()
	{		
		if (0 < mDashCount.CurDashCount)
		{
			mDashCount.CurDashCount -= 1;
			mDashCountBar->GetComponent<DashCountScript>()->BindConstantBuffer();
			return true;
		}
		mDashCountBar->GetComponent<DashCountScript>()->BindConstantBuffer();
		return false;
	}

	void GameDataManager::RecoveryDash()
	{
		if (mDashCount.MaxDashCount > mDashCount.CurDashCount)
		{
			mDashCount.CurDashCount += 1;
			mDashCountBar->GetComponent<DashCountScript>()->BindConstantBuffer();
		}


	}

	void GameDataManager::CallInventory()
	{
		mInventoryObject->GetComponent<InventoryScript>()->CallInventory();		
	}

	void GameDataManager::ChangeArmour()
	{
		// swap armour
		structs::sArmour temp = mActiveArmour;
		mActiveArmour = mSubArmour;
		mSubArmour = temp;

		// change UI
		mInventoryObject->GetComponent<InventoryScript>()->ChangeArmour();
	}
}