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
		static structs::sCreatureStat& GetPlayerStat() { return mPlayerStat; }
		static structs::sDashCount& GetDashCount() { return mDashCount; }
		static structs::sInventory& GetInventory() { return mInventory; }

	private:
		static structs::sCreatureStat mPlayerStat;
		static structs::sDashCount mDashCount;
		static structs::sInventory mInventory;
	};
}

/*
	���� ������ ���������� ���� HUD �� Overlay UI ��ü�� ������ �ְ�?(��ü������ ������)
	�÷��̾��� ���ݰ� ������ �� �÷��̾� ���� ������ ������ �־�� �ϸ�?
	���� ���� ��Ģ�� ������ �־���� (���� ������ �ѱ�� ����̶����)
	�ð� ������ ������ �־���Ҷ�? �÷��� �ð� ������
*/