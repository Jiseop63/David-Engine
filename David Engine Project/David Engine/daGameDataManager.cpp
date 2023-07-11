#include "daGameDataManager.h"

namespace da
{
	using namespace math;
	structs::eCreatureStat GameDataManager::mPlayerStat;


	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.maxHP = 30;
		mPlayerStat.curHP = mPlayerStat.maxHP;
	}

}