#pragma once
#include "daCreatureScript.h"

namespace da
{
	class SkelScript : public CreatureScript
	{
	public:
		SkelScript();
		virtual ~SkelScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		void chasingTimeout();
		void attackCooldownReady();

#pragma region Spawn
		// 초기 위치와 스텟 정하기
		// 등장 이펙트? 애니메이션? 진행하기
#pragma endregion


#pragma region FSM
		void ChangeState(eCreatureState state);
		void SkelFSM();
		void SkelHandleIdle();
		void SkelHandleChase();
		void SkelHandleAttack();
		void SkelHandleDead();
#pragma endregion

#pragma region Chase Func
		void delayChase();
		
		float calcCreatureDir(math::Vector3 targetPosition, math::Vector3 myPosition);
		void returnIdle();
		void moveToAttackRange();
#pragma endregion

#pragma region Attack Func
		void delayAttack();
		void doAttack();
#pragma endregion



#pragma region Collision

#pragma endregion
#pragma region RetIdle condition value
	private:
		bool			mGotoReturn;
		float			mReturnAccumulateTime;
		float			mReturnDelayTime;
#pragma endregion

#pragma region AttackCooldown condition value
	
#pragma endregion


	};
}