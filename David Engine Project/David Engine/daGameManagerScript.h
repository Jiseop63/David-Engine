#pragma once
#include "daScript.h"
#include "daInventoryScript.h"
namespace da
{
	class GameManagerScript : public Script
	{
	public:
		GameManagerScript();
		virtual ~GameManagerScript();

		virtual void Update() override;
		
		void InventoryOpen();
		void InventoryClose();

	public:
		void SetInventory(InventoryScript* script) { mInventory = script; }

	private:
		bool mInventoryOpen;

		InventoryScript* mInventory;
	};
}