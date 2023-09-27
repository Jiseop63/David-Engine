#pragma once
#include "daMonsterScript.h"

namespace da
{
	enum class eBansheeState
	{
		Idle,
		Attack,
		Dead,
	};

	class BansheeScript : public MonsterScript
	{
	public:
		BansheeScript();
		virtual ~BansheeScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		virtual void AddActionUnit(GameObject* unit) override;
		CombatScript* AddCombatObject(GameObject* object);


#pragma region Spawn
		// 초기 위치와 스텟 정하기
		// 등장 이펙트? 애니메이션? 진행하기

		void retIdle();
#pragma endregion

#pragma region Common Func
	private:
		void calcTargetDir();
		void lifeCheck();
#pragma endregion

#pragma region FSM
	private:
		void ChangeState(eBansheeState state);
		void monsterFSM();
		void BansheeHandleIdle();
		void BansheeHandleAttack();
		void BansheeHandleDead();
#pragma endregion

#pragma region Chase Func
	private:
		void findingPlayer();		// 추격 제한로직
#pragma endregion

#pragma region Attack Func
	private:
		void prepareForAttack();	// 공격 선딜
		void doAttack();			// 공격 진행
		void readyForAttackDelay(); // 공격 후딜
#pragma endregion



#pragma region Collision

#pragma endregion
		eBansheeState mMonsterActiveState;
#pragma region Chase Value
	private:
		float	mChaseDurationTime;			// 최대 추적 유지시간
		float	mChaseDurationDecay;		// 현재 유지시간
		float	mDistanceFromPlayer;		// 플레이어와의 거리
#pragma endregion

#pragma region AttackCooldown condition value
		bool	mPrepareAttack;
		float	mPrepareDurationTime;
		float	mPrepareDurationDecay;
		bool	mAttackProgress;
		float	mReadyDurationTime;
		float	mReadyDurationDecay;
#pragma endregion

		bool	mIsAttacked;
	};
}