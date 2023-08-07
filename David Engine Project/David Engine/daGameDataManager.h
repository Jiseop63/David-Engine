#pragma once
#include "daGameObject.h"
// or David Engine.h
namespace da
{
	class GameDataManager
	{
	public:
		static void Initialize();		
		static void InitializePlayerStat();

	public:
		static structs::sPlayerStat& GetPlayerStat() { return mPlayerStat; }
		static structs::sJumpCount& GetJumpCount() { return mJumpCount; }
		static structs::sDashCount& GetDashCount() { return mDashCount; }
		static structs::sInventory& GetInventory() { return mInventoryData; }

#pragma region Player Func
	public:
		static void GetDamage(float value);
		static void GetHeal(float value);

		static bool UseDash();
		static void RecoveryDash();
		static void ClearJumpBuffer();
#pragma endregion

#pragma region Inventory Func
	public:
		static void CallInventory();
		static void ChangeArmour();
		static structs::sArmour* GetActiveArmour();
#pragma endregion

#pragma region Data
	private:
		static structs::sPlayerStat mPlayerStat;
		static structs::sJumpCount mJumpCount;
		static structs::sDashCount mDashCount;
		static structs::sInventory mInventoryData;

		static structs::sArmour* mActiveArmour;
		static structs::sArmour* mSubArmour;
#pragma endregion
	};
}