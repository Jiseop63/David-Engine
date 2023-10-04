#include "daInventoryScript.h"

#include "daInput.h"

#include "daGameObject.h"

#include "daMeshRenderer.h"


namespace da
{
	int InventoryScript::itemSlot = 0;
	InventoryScript::InventoryScript()
		: mActiveItemSlot(nullptr)
		, mMainArmourScript(nullptr)
		, mSubArmourScript(nullptr)
		, mInventoryOpen(false)
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
		mMainArmourScript->ChangeArmour();
		mSubArmourScript->ChangeArmour();
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
		// �κ��丮�� ����
		if (mInventoryOpen)
		{
			// ������ ���� ��ġ�� �ִ� �������� ���õ�
			if (Input::GetKeyDown(eKeyCode::LBTN))
			{
				// ������ ��ȸ�ϸ� focus�� ���� ��ü�� ã��
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
				// ������ ��ȸ�ϸ� focus�� ���� ��ü�� ã��
				for (UINT slotIndex = 0; slotIndex < (UINT)enums::eItemSlot::End; slotIndex++)
				{
					if (mItemSlots[slotIndex]->IsFocus())
					{
						mSwapIndex.Second = slotIndex;
					}
				}
			}
			
			// �� �� �ʱ���°� �ƴ�
			if (-1 != mSwapIndex.First && -1 != mSwapIndex.Second)
			{
				// �� ���ڰ� ���� ������
				if (mSwapIndex.First == mSwapIndex.Second)
				{
					mSwapIndex.Clear(); // ����ġ
				}
				else
				{
					ChangeItems(); // ����
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

		// ���Կ� �߰���
		if (enums::eItemSlot::Slot00 == (enums::eItemSlot)mSwapIndex.Second)
		{
			mMainArmourScript->SetSlotScript(mItemSlots[mSwapIndex.Second]);
			mMainArmourScript->ChangeIcon();
		}
		if (enums::eItemSlot::Slot02 == (enums::eItemSlot)mSwapIndex.Second)
		{
			mSubArmourScript->SetSlotScript(mItemSlots[mSwapIndex.Second]);
			mSubArmourScript->ChangeIcon();
		}
		// ���Կ� �����
		if (enums::eItemSlot::Slot00 == (enums::eItemSlot)mSwapIndex.First)
		{
			mMainArmourScript->SetSlotScript(mItemSlots[mSwapIndex.First]);
			mMainArmourScript->ChangeIcon();
		}
		if (enums::eItemSlot::Slot02 == (enums::eItemSlot)mSwapIndex.First)
		{
			mSubArmourScript->SetSlotScript(mItemSlots[mSwapIndex.First]);
			mSubArmourScript->ChangeIcon();
		}
		mSwapIndex.Clear();
	}

	ItemScript* InventoryScript::GetActiveItemScript()
	{		
		return mActiveItemSlot->GetItemScript();
	}
	void InventoryScript::AddItemToSlot(enums::eItemSlot slot, ItemScript* item)
	{
		int a = 0;
		mItemSlots[(UINT)slot]->SetItemScript(item);
		mItemSlots[(UINT)slot]->ClearPosition();

		//// �̹� ������ �Ҵ�ǰ�, �гε� ������ �����̹Ƿ� �������� ���ĭ�� �߰��ɶ� �гο� �������ָ� �ɵ�

		if (enums::eItemSlot::Slot00 == slot)
		{
			mMainArmourScript->SetSlotScript(mItemSlots[(UINT)slot]);
			mMainArmourScript->ChangeIcon();
		}
		if (enums::eItemSlot::Slot02 == slot)
		{
			mSubArmourScript->SetSlotScript(mItemSlots[(UINT)slot]);
			mSubArmourScript->ChangeIcon();
		}

	}
}