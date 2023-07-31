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




		// Scene::Enter 에서 호출해줘야함
	public:
		static void SetInventoryObject(GameObject* obj) { mInventoryObject = obj; }
		static void SetWeaponObject(GameObject* obj) { mWeaponObject = obj; }
		static void SetPlayerObject(GameObject* obj) { mPlayerObject = obj; }
		
		static void SetHPBar(GameObject* obj) { mHPBar = obj; }
		static void SetDashCountBar(GameObject* obj) { mDashCountBar = obj; }

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
		static bool UseJump();
		static void RecoveryJump();
		static void ResetJumpBuffer();
#pragma endregion

#pragma region Inventory Func
	public:
		static void CallInventory();
		static void ChangeArmour();

#pragma endregion

#pragma region Data
	private:
		static structs::sPlayerStat mPlayerStat;
		static structs::sJumpCount mJumpCount;
		static structs::sDashCount mDashCount;
		static structs::sInventory mInventoryData;

		static structs::sArmour mActiveArmour;
		static structs::sArmour mSubArmour;
#pragma endregion


#pragma region Objects
	private:
		static GameObject* mInventoryObject;
		static GameObject* mWeaponObject;
		static GameObject* mPlayerObject;

		static GameObject* mHPBar;
		static GameObject* mDashCountBar;
#pragma endregion
	};
}

/*
	여러 씬에서 공통적으로 쓰일 HUD 와 Overlay UI 객체를 가지고 있고?(자체적으로 관리함)
	플레이어의 스텟과 아이템 등 플레이어 관련 정보를 가지고 있어야 하며?
	게임 진행 규칙을 가지고 있어야함 (다음 씬으로 넘기는 기능이라던가)
	시간 정보도 가지고 있어야할뜻? 플레이 시간 같은거
*/