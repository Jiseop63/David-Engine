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




		// Scene::Enter ���� ȣ���������
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
	���� ������ ���������� ���� HUD �� Overlay UI ��ü�� ������ �ְ�?(��ü������ ������)
	�÷��̾��� ���ݰ� ������ �� �÷��̾� ���� ������ ������ �־�� �ϸ�?
	���� ���� ��Ģ�� ������ �־���� (���� ������ �ѱ�� ����̶����)
	�ð� ������ ������ �־���Ҷ�? �÷��� �ð� ������
*/