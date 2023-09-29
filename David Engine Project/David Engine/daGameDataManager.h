#pragma once
#include "daGameObject.h"
// or David Engine.h


namespace da
{
	enum class eDungeonScene
	{
		F1Stage2,
		F1Stage3,
		End,
	};
	class GameDataManager
	{
	public:
		static void Initialize();		
		static void InitializeInventory();

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
		static void InitializeMonsterCount(eDungeonScene scene, int value) { mMonsterCounts[(UINT)scene] = value; }
		
		static void EnterMonsterCount(eDungeonScene scene, std::vector<class PortalScript*> portals);
		static void ExitMonsterCount(eDungeonScene scene);
		static void DecreaseMonsterCount(std::vector<class PortalScript*> portals);

	public:
		static structs::sInventory& GetInventory() { return mInventoryData; }

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
		static structs::sArmour* GetActiveArmour();
#pragma endregion

#pragma region Data
	private:
		static structs::sInventory		mInventoryData;

		static structs::sArmour*		mActiveArmour;
		static structs::sArmour*		mSubArmour;

		static da::math::Vector2		mCameraMovableRange;
		static da::math::Vector2		mCameraMovePosition;

		static bool mDebuging;
		static std::vector<int>	mMonsterCounts;
		static int				mCurrentMonsterCount;
#pragma endregion
	};
}