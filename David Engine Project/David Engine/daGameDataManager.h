#pragma once
#include "daGameObject.h"
// or David Engine.h


namespace da
{
	enum class eDungeonScene
	{
		//dungeon1_1,
		dungeon1_2,
		dungeon1_3,
		dungeon1_4,
		dungeon1_5,
		dungeon1_final,
		//dungeon2_1,
		dungeon2_boss,

		/*F1Stage2,
		F1Stage3,
		F2Stage2,*/
		End,
	};
	class GameDataManager
	{
		// debug render
	public:
		static void DebugMode()
		{
			if (mDebuging)
				mDebuging = false; 
			else
				mDebuging = true;
		}
		static bool IsDebuging() { return mDebuging; }
		
		// Camera movement
	public:		
		static void SetCameraMovableRange(da::math::Vector2 vector2) { mCameraMovableRange = vector2; } // Dx기준 좌표계 사용
		static da::math::Vector2 GetCameraMovableRange() { return mCameraMovableRange; }

		static void SetCameraMovaPosition(da::math::Vector2 vector2, bool shake = false);
		static da::math::Vector2 GetCameraMovaPosition() { return mCameraMovePosition; }

		// Monster Count
	public:
		static void InitializeMonsterCount(eDungeonScene scene, int value) { mMonsterCounts[(UINT)scene] = value; }

		static void EnterMonsterCount(eDungeonScene scene, std::vector<class PortalScript*> portals);
		static void ExitMonsterCount(eDungeonScene scene);
		static void DecreaseMonsterCount(std::vector<class PortalScript*> portals);

	private:
		static da::math::Vector2		mCameraMovableRange;
		static da::math::Vector2		mCameraMovePosition;

		static bool mDebuging;
		static std::vector<int>	mMonsterCounts;
		static int				mCurrentMonsterCount;
	};
}