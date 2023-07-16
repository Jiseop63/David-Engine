#include "daGameDataManager.h"

namespace da
{
	using namespace math;
	structs::sCreatureStat GameDataManager::mPlayerStat = {};
	structs::sDashCount GameDataManager::mDashCount = {};

	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 6.0f;
		mDashCount.MaxCount = 2.0f;
		mDashCount.CurCount = mDashCount.MaxCount;
	}

}