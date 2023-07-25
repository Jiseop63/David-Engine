#include "daGameDataManager.h"

#include "daInventoryScript.h"

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

	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 6.0f;
		mDashCount.MaxCount = 2.0f;
		mDashCount.CurCount = mDashCount.MaxCount;

		mInventoryData.Armour1.Weapon = enums::eWeaponType::LongSword;
		mActiveArmour = mInventoryData.Armour1;
		mSubArmour = mInventoryData.Armour2;
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