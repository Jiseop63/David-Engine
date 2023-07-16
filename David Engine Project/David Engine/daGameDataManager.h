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
	private:
		static structs::sCreatureStat mPlayerStat;
		static structs::sDashCount mDashCount;
	};
}

/*
	���� ������ ���������� ���� HUD �� Overlay UI ��ü�� ������ �ְ�?(��ü������ ������)
	�÷��̾��� ���ݰ� ������ �� �÷��̾� ���� ������ ������ �־�� �ϸ�?
	���� ���� ��Ģ�� ������ �־���� (���� ������ �ѱ�� ����̶����)
	�ð� ������ ������ �־���Ҷ�? �÷��� �ð� ������
*/