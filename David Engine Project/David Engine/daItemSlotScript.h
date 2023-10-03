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

		virtual void Initialize() override;


	public:
		void SetItemSlot(enums::eItemSlot slot) { mItemSlot = slot; }
		void SetItemScript(ItemScript* itemScript) { mItemScript = itemScript; }

		void ClearPosition() { mItemScript->SetPosition(mSlotTransform->GetPosition()); }
		ItemScript* GetItemScript() { return mItemScript; }
		bool IsFocus() { return mFocusOn; }
		bool HasItem() { return (nullptr != mItemScript) ? true : false; }

	protected:
		Transform*		mSlotTransform;
		MeshRenderer*	mSlotRenderer;

		// ������ �Ӽ�
		enums::eItemSlot	mItemSlot;
		ItemScript*		mItemScript;
	};
}
