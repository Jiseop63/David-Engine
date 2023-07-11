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
		static structs::eCreatureStat& GetPlayerStat() { return mPlayerStat; }

	private:
		static structs::eCreatureStat mPlayerStat;
	};
}

/*
	���� ������ ���������� ���� HUD �� Overlay UI ��ü�� ������ �ְ�?(��ü������ ������)
	�÷��̾��� ���ݰ� ������ �� �÷��̾� ���� ������ ������ �־�� �ϸ�?
	���� ���� ��Ģ�� ������ �־���� (���� ������ �ѱ�� ����̶����)
	�ð� ������ ������ �־���Ҷ�? �÷��� �ð� ������
*/