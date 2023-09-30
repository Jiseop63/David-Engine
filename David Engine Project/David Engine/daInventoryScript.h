#pragma once
#include "daScript.h"
#include "daGameObject.h"
#include "daTexture.h"

namespace da
{
	enum class ItemSlot
	{
		
	};

	class InventoryScript : public Script
	{
	public:
		InventoryScript();
		virtual ~InventoryScript();

		virtual void Update() override;
		
		void CallInventory();
		void ChangeArmour();
		void Open();
		void Close();

	public:
		void SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture);

		void AddWeaonSlot(GameObject* weapon) { mWeapon.push_back(weapon); }
		void AddShiledSlot(GameObject* shiled) { mShiled.push_back(shiled); }
		void AddAccessorySlot(GameObject* accessory) { mAccessory.push_back(accessory); }
		void AddItemSlot(GameObject* item) { mItem.push_back(item); }

	protected:
		bool mInventoryOpen;
		bool mSelectLeft;
		
		// 무기 2개 방패 2개 장신구 4개 아이템 15개
		std::vector<GameObject*> mWeapon;
		std::vector<GameObject*> mShiled;
		std::vector<GameObject*> mAccessory;
		std::vector<GameObject*> mItem;

	private:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;
	};
}
