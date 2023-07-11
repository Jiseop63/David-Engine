#include "daGameManagerScript.h"

#include "daInput.h"

#include "daInventoryScript.h"

namespace da
{
	GameManagerScript::GameManagerScript()
		: mInventoryOpen(false)
		, mInventory(nullptr)
	{
	}
	GameManagerScript::~GameManagerScript()
	{
	}
	void GameManagerScript::Update()
	{
		
		// 인벤토리
		if (nullptr != mInventory 
			&& Input::GetKeyDown(eKeyCode::V))
		{
			if (false == mInventoryOpen)
				InventoryOpen();
			else
				InventoryClose();
		}

		if (Input::GetKeyDown(eKeyCode::T))
		{
			int getDamage = 5.0f;
			if (! (mPlayerStat.curHP <= getDamage) )
				mPlayerStat.curHP -= 5.0f;
		}
	}
	void GameManagerScript::InventoryOpen()
	{
		mInventoryOpen = true;
		mInventory->Open();
	}
	void GameManagerScript::InventoryClose()
	{
		mInventoryOpen = false;
		mInventory->Close();
	}
}