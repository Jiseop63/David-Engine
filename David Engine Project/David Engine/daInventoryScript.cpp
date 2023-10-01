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
		}
		else
		{
			mInventoryOpen = false;
			GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
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
		itemSlotScript->SetItemSlot((enums::ItemSlot)itemSlot++);
		mItemSlots.push_back(itemSlotScript);
		itemSlotScript->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
		return itemSlotScript;
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
				for (UINT slotIndex = 0; slotIndex < (UINT)enums::ItemSlot::End; slotIndex++)
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
				for (UINT slotIndex = 0; slotIndex < (UINT)enums::ItemSlot::End; slotIndex++)
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
		ItemSlotScript* tempSlot = mItemSlots[mSwapIndex.First];
		mItemSlots[mSwapIndex.First] = mItemSlots[mSwapIndex.Second];
		mItemSlots[mSwapIndex.Second] = tempSlot;
		mSwapIndex.Clear();
	}

	ItemScript* InventoryScript::GetActiveItemScript()
	{		
		return mActiveItemSlot->GetItemScript();
	}
}