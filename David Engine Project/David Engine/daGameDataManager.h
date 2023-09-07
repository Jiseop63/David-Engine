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
		static void DebugMode()
		{
			if (mDebuging)
				mDebuging = false; 
			else
				mDebuging = true;
		}
		static bool IsDebuging() { return mDebuging; }

	public:
		static structs::sCreatureStat& GetPlayerStat() { return mPlayerStat; }
		static structs::sJumpData& GetJumpCount() { return mJumpData; }
		static structs::sDashData& GetDashCount() { return mDashData; }
		static structs::sInventory& GetInventory() { return mInventoryData; }

#pragma region Player Func
	public:
		static void GetDamage(float value);
		static void GetHeal(float value);

		static bool UseDash();
		static void RecoveryDash();
		static void ClearJumpBuffer();
#pragma endregion

#pragma region Scene Enter Func
	public:
		// Dx기준 좌표계 사용 ㄱㄱ
		static void SetCameraMovableRange(da::math::Vector2 vector2) { mCameraMovableRange = vector2; }
		static da::math::Vector2 GetCameraMovableRange() { return mCameraMovableRange; }

		static void SetCameraMovaPosition(da::math::Vector2 vector2, bool shake = false);
		static da::math::Vector2 GetCameraMovaPosition() { return mCameraMovePosition; }
#pragma endregion

#pragma region Inventory Func
	public:
		static void CallInventory();
		static void ChangeArmour();
		static structs::sArmour* GetActiveArmour();
#pragma endregion

#pragma region Data
	private:
		static structs::sCreatureStat mPlayerStat;
		static structs::sJumpData mJumpData;
		static structs::sDashData mDashData;
		static structs::sInventory mInventoryData;

		static structs::sArmour* mActiveArmour;
		static structs::sArmour* mSubArmour;

		static da::math::Vector2 mCameraMovableRange;
		static da::math::Vector2 mCameraMovePosition;


		static bool mDebuging;
#pragma endregion
	};
}