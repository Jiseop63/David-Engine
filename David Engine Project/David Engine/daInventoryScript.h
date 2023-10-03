#pragma once
#include "daScript.h"
#include "daGameObject.h"
#include "daTexture.h"
#include "daItemSlotScript.h"
#include "daArmourScript.h"

namespace da
{
	class InventoryScript : public Script
	{
		struct sSwapNumber
		{
			int	First;
			int Second;
			sSwapNumber() : First(-1), Second(-1)
			{}
			void Clear() { First = -1; Second = -1; }
		};
	public:
		InventoryScript();
		virtual ~InventoryScript();
				
		virtual void Update() override;

#pragma region visual
	public:
		void SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture);

	public:
		void CallInventory();
		void ChangeArmour();
#pragma endregion

		ItemSlotScript* AddItemSlotScript(GameObject* itemObject);
		ArmourScript* AddArmourScript(GameObject* armourObject);

#pragma region Item
		void SelectForSwap();
		void ChangeItems();
		ItemScript* GetActiveItemScript();
		void AddItemToSlot(enums::eItemSlot slot, ItemScript* item);
#pragma endregion



	protected:
		bool mInventoryOpen;
		bool mSelectLeft;
		
	private:
		static int							itemSlot;
		std::shared_ptr<graphics::Texture>	mFirstTexture;
		std::shared_ptr<graphics::Texture>	mSecondTexture;

		std::vector<ItemSlotScript*>		mItemSlots;

		ItemSlotScript*						mActiveItemSlot;

		sSwapNumber							mSwapIndex;


		ArmourScript*						mMainArmourScript;
		ArmourScript*						mSubArmourScript;

	};
}
