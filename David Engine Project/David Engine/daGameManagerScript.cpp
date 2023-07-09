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
		
		// �κ��丮
		if (nullptr != mInventory 
			&& Input::GetKeyDown(eKeyCode::V))
		{
			if (false == mInventoryOpen)
				InventoryOpen();
			else
				InventoryClose();
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