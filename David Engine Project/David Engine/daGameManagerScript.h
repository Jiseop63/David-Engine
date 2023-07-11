#pragma once
#include "daScript.h"
#include "daInventoryScript.h"
namespace da
{
	class GameManagerScript : public Script
	{
	public:
		struct eCreatureStat
		{
			float maxHP;
			float curHP;
		};

	public:
		GameManagerScript();
		virtual ~GameManagerScript();

		virtual void Update() override;
		
		void InventoryOpen();
		void InventoryClose();

		eCreatureStat GetPlayerStat() { return mPlayerStat; }

	public:
		void SetInventory(InventoryScript* script) { mInventory = script; }

	private:
		bool mInventoryOpen;
		InventoryScript* mInventory;

	private:
		eCreatureStat mPlayerStat;
	};
}