#include "daItemManager.h"

namespace da
{
	std::map<const std::wstring, ItemScript*> ItemManager::mItems = {};

	ItemScript* ItemManager::GetItem(const std::wstring name)
	{
		auto target =  mItems.find(name);		
		return target->second;
	}
	void ItemManager::AddItem(const std::wstring name, ItemScript* item)
	{
		mItems.insert(std::make_pair(name, item));
	}
}