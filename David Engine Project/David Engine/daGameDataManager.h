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
	여러 씬에서 공통적으로 쓰일 HUD 와 Overlay UI 객체를 가지고 있고?(자체적으로 관리함)
	플레이어의 스텟과 아이템 등 플레이어 관련 정보를 가지고 있어야 하며?
	게임 진행 규칙을 가지고 있어야함 (다음 씬으로 넘기는 기능이라던가)
	시간 정보도 가지고 있어야할뜻? 플레이 시간 같은거
*/