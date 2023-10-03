#include "daInventoryScript.h"

#include "daInput.h"

#include "daGameObject.h"

#include "daMeshRenderer.h"


namespace da
{
	int InventoryScript::itemSlot = 0;
	InventoryScript::InventoryScript()
		: mInventoryOpen(false)
		, mSelectLeft(true)
	{
	}
	InventoryScript::~InventoryScript()
	{
	}

	void InventoryScript::Update()
	{
		SelectForSwap();		
	}

	void InventoryScript::CallInventory()
	{
		if (false == mInventoryOpen)
		{
			mInventoryOpen = true;
			GetOwner()->SetObjectStates(GameObject::eObjectState::Active);
			for (ItemSlotScript* slot : mItemSlots)
			{
				if (slot->HasItem())
				{
					slot->GetItemScript()->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
				}
			}
		}
		else
		{
			mInventoryOpen = false;
			GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
			for (ItemSlotScript* slot : mItemSlots)
			{
				if (slot->HasItem())
				{
					slot->GetItemScript()->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
				}
			}
		}
	}
	void InventoryScript::ChangeArmour()
	{
		if (true == mSelectLeft)
		{
			ChangeSlotTexture(mSecondTexture);
			mSelectLeft = false;
		}
		else
		{
			ChangeSlotTexture(mFirstTexture);
			mSelectLeft = true;
		}
	}
	void InventoryScript::SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeMaterialTexture(first);
	}
	void InventoryScript::ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeMaterialTexture(texture);
	}

	ItemSlotScript* InventoryScript::AddItemSlotScript(GameObject* itemObject)
	{
		ItemSlotScript* itemSlotScript = itemObject->AddComponent<ItemSlotScript>();
		itemSlotScript->SetItemSlot((enums::eItemSlot)itemSlot++);
		mItemSlots.push_back(itemSlotScript);
		itemSlotScript->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		return itemSlotScript;
	}
	ArmourScript* InventoryScript::AddArmourScript(GameObject* armourObject)
	{
		if (!mMainArmourScript)
		{
			mMainArmourScript = armourObject->AddComponent<ArmourScript>();
			return mMainArmourScript;
		}
		else
		{
			mSubArmourScript = armourObject->AddComponent<ArmourScript>();
			return mSubArmourScript;
		}
	}
	void InventoryScript::SelectForSwap()
	{
		// 인벤토리가 열림
		if (mInventoryOpen)
		{
			// 누르면 누른 위치에 있는 아이템이 선택됨
			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				// 슬롯을 순회하며 focus를 가진 객체를 찾음
				for (UINT slotIndex = 0; slotIndex < (UINT)enums::eItemSlot::End; slotIndex++)
				{
					if (mItemSlots[slotIndex]->IsFocus())
					{
						mSwapIndex.First = slotIndex;
					}
				}
			}
			if (Input::GetKeyUp(eKeyCode::LBTN))
			{
				// 슬롯을 순회하며 focus를 가진 객체를 찾음
				for (UINT slotIndex = 0; slotIndex < (UINT)enums::eItemSlot::End; slotIndex++)
				{
					if (mItemSlots[slotIndex]->IsFocus())
					{
						mSwapIndex.Second = slotIndex;
					}
				}
			}
			
			// 둘 다 초기상태가 아님
			if (-1 != mSwapIndex.First && -1 != mSwapIndex.Second)
			{
				// 두 숫자가 같은 숫자임
				if (mSwapIndex.First == mSwapIndex.Second)
				{
					mSwapIndex.Clear(); // 원위치
				}
				else
				{
					ChangeItems(); // 스왑
				}
			}
		}
	}
	void InventoryScript::ChangeItems()
	{
		ItemScript* tempItem = mItemSlots[mSwapIndex.First]->GetItemScript();
		mItemSlots[mSwapIndex.First]->SetItemScript(mItemSlots[mSwapIndex.Second]->GetItemScript());
		mItemSlots[mSwapIndex.Second]->SetItemScript(tempItem);

		if (mItemSlots[mSwapIndex.First]->GetItemScript())
			mItemSlots[mSwapIndex.First]->ClearPosition();
		if (mItemSlots[mSwapIndex.Second]->GetItemScript())
			mItemSlots[mSwapIndex.Second]->ClearPosition();
		mSwapIndex.Clear();

	}

	ItemScript* InventoryScript::GetActiveItemScript()
	{		
		return mActiveItemSlot->GetItemScript();
	}
	void InventoryScript::AddItemToSlot(enums::eItemSlot slot, ItemScript* item)
	{
		mItemSlots[(UINT)slot]->SetItemScript(item);
		mItemSlots[(UINT)slot]->ClearPosition();
	}
}