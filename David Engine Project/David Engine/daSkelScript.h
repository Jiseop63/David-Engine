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

#pragma region FSM
		void ChangeState(eCreatureState state);
		void SkelFSM();
		void SkelHandleIdle();
		void SkelHandleChase();
		void SkelHandleAttack();
		void SkelHandleDead();
#pragma endregion

#pragma region Collision

#pragma endregion
	private:
		bool			mGotoReturn;
		float			mReturnAccumulateTime;
		float			mReturnDelayTime;
	};
}