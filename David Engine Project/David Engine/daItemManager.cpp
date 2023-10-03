#include "daItemManager.h"

namespace da
{
	std::map<const std::wstring, ItemScript*> ItemManager::mItems = {};

	void ItemManager::Initialize()
	{
		//ItemScript* defalutWeapon = new ItemScript();
		//structs::sUnitTypes defalutWeaponTypes;
		//defalutWeaponTypes.ActionType = enums::eUnitActionType::None;
		//defalutWeaponTypes.RenderType = enums::eUnitRenderType::JustRotate;
		//defalutWeaponTypes.UsageType = enums::eUnitUsageType::Default;
		//defalutWeapon->SetUnitTypes(defalutWeaponTypes);
		//structs::sActionUnitInfo defalutWeaponInfo;
		//defalutWeaponInfo.DurationTime = structs::sActionTimeValues(); // ±âº»°ª 0 - 1
		//defalutWeaponInfo.Range = 0.50f;
		//defalutWeaponInfo.Scale = 1.60f;
		//defalutWeaponInfo.Speed = 0.0f;
		//defalutWeapon->SetUnitInfo(defalutWeaponInfo);
		//defalutWeapon->SetUnitAnimation(L"Swing", false);
		//defalutWeapon->SetItemName(L"LongSword");

		//mItems.insert(std::make_pair(L"LongSword", defalutWeapon));
	}
	void ItemManager::Release()
	{
		/*std::map<const std::wstring, ItemScript*>::iterator iter;
		for (iter = mItems.begin(); iter != mItems.end(); ++iter)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		mItems.clear();*/
	}
	void ItemManager::CallInventory()
	{
	}
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