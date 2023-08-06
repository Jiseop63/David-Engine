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

#pragma region FSM
		void HandleIdle();
		void HandleChase();
		void HandleAttack();
		void HandleDead();
#pragma endregion

#pragma region Collision
		virtual void OnCollisionEnter(Collider2D* other);

#pragma endregion

	};
}