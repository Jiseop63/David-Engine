#pragma once
#include "daUIScript.h"
#include "daTexture.h"
#include "daMeshRenderer.h"
#include "daItemScript.h"

namespace da
{
	class ItemSlotScript : public UIScript
	{
		enum class eItemType
		{
			Weapon,
			Shiled,
			Accessory,
			Item,
		};
	public:
		ItemSlotScript();
		virtual ~ItemSlotScript();

	public:
		void SetItemSlot(enums::ItemSlot slot) { mItemSlot = slot; }
		void SetItemScript(ItemScript* itemScript) { mItemScript = itemScript; }

		ItemScript* GetItemScript() { return mItemScript; }
		bool IsFocus() { return mFocusOn; }

	protected:
		Transform*		mSlotTransform;
		MeshRenderer*	mSlotRenderer;

		// 아이템 속성
		enums::ItemSlot	mItemSlot;
		ItemScript*		mItemScript;
	};
}
