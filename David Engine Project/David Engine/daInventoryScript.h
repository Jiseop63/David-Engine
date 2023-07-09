#pragma once
#include "daScript.h"
#include "daGameObject.h"
namespace da
{
	class InventoryScript : public Script
	{
	public:
		InventoryScript();
		virtual ~InventoryScript();

		virtual void Update() override;
		
		void Open();
		void Close();

	protected:
		// 무기 2개
		std::vector<GameObject*> mWeapon;
		// 방패 2개
		std::vector<GameObject*> mShiled;
		// 장신구 4개
		std::vector<GameObject*> mAccessory;
		// 아이템 15개
		std::vector<GameObject*> mItem;
	};
}
