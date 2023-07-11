#include "daInventoryScript.h"

#include "daGameObject.h"

#include "daInput.h"

namespace da
{
	InventoryScript::InventoryScript()
		: mInventoryOpen(false)
		, mWeapon{}
		, mShiled{}
		, mAccessory{}
		, mItem{}
	{
	}
	InventoryScript::~InventoryScript()
	{
	}
	void InventoryScript::Update()
	{
		// 인벤토리
		if (Input::GetKeyDown(eKeyCode::V))
		{
			if (false == mInventoryOpen)
			{
				mInventoryOpen = true;
				Open();
			}
			else
			{
				mInventoryOpen = false;
				Close();
			}
		}
	}
	void InventoryScript::Open()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void InventoryScript::Close()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
	}
}