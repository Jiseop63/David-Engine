#include "daGameDataManager.h"

#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"

namespace da
{
	using namespace math;
	structs::sCreatureStat GameDataManager::mPlayerStat = {};
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
		mPlayerStat.MoveSpeed = 6.0f;
		mDashCount.MaxCount = 2;
		mDashCount.CurCount = mDashCount.MaxCount;

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
		if (0 < mDashCount.CurCount)
		{
			mDashCount.CurCount -= 1;
			mDashCountBar->GetComponent<DashCountScript>()->BindConstantBuffer();
			return true;
		}
		mDashCountBar->GetComponent<DashCountScript>()->BindConstantBuffer();
		return false;
	}

	void GameDataManager::RecoveryDash()
	{
		if (mDashCount.MaxCount > mDashCount.CurCount)
		{
			mDashCount.CurCount += 1;
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