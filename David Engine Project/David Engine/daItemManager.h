#pragma once
#include "daGameObject.h"
#include "daItemScript.h"
// or David Engine.h


namespace da
{
	class ItemManager
	{
	public:
		static void Initialize();
		static void Release();

	public:
		static void CallInventory();
		static ItemScript* GetItem(const std::wstring name);
		static void AddItem(const std::wstring name, ItemScript* item);


	private:
		static std::map<const std::wstring, ItemScript*> mItems;
	};
}