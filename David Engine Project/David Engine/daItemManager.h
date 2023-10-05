#pragma once
#include "daGameObject.h"
#include "daItemScript.h"
#include "daInventoryScript.h"
// or David Engine.h


namespace da
{
	class ItemManager
	{
	public:
		static ItemScript* GetItem(const std::wstring name);
		static void AddItem(const std::wstring name, ItemScript* item);

	private:
		static std::map<const std::wstring, ItemScript*> mItems;
	};
}