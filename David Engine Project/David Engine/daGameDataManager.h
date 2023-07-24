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

	public:
		static structs::sCreatureStat& GetPlayerStat() { return mPlayerStat; }
		static structs::sDashCount& GetDashCount() { return mDashCount; }
		static structs::sInventory& GetInventory() { return mInventoryData; }

	private:
		static structs::sCreatureStat mPlayerStat;
		static structs::sDashCount mDashCount;
		static structs::sInventory mInventoryData;

	private:
		static GameObject* mInventoryObject;
		static GameObject* mWeaponObject;
		static GameObject* mPlayerObject;
	};
}

/*
	���� ������ ���������� ���� HUD �� Overlay UI ��ü�� ������ �ְ�?(��ü������ ������)
	�÷��̾��� ���ݰ� ������ �� �÷��̾� ���� ������ ������ �־�� �ϸ�?
	���� ���� ��Ģ�� ������ �־���� (���� ������ �ѱ�� ����̶����)
	�ð� ������ ������ �־���Ҷ�? �÷��� �ð� ������
*/