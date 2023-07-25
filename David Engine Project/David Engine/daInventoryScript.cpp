#include "daInventoryScript.h"

#include "daGameObject.h"

#include "daInput.h"
#include "daMeshRenderer.h"

namespace da
{
	InventoryScript::InventoryScript()
		: mInventoryOpen(false)
		, mSelectLeft(true)
		, mWeapon{}
		, mShiled{}
		, mAccessory{}
		, mItem{}
	{
	}
	InventoryScript::~InventoryScript()
	{
	}
	void InventoryScript::Update()
	{
	}

	void InventoryScript::CallInventory()
	{
		if (false == mInventoryOpen)
		{
			mInventoryOpen = true;
			Open();
		}
		else
		{
			mInventoryOpen = false;
			Close();
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
	void InventoryScript::Open()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);

		for (GameObject* weapon : mWeapon)
		{
			weapon->SetObjectState(GameObject::eObjectState::Active);
		}
		for (GameObject* shiled : mShiled)
		{
			shiled->SetObjectState(GameObject::eObjectState::Active);
		}
		for (GameObject* accessory : mAccessory)
		{
			accessory->SetObjectState(GameObject::eObjectState::Active);
		}
		for (GameObject* item : mItem)
		{
			item->SetObjectState(GameObject::eObjectState::Active);
		}
	}
	void InventoryScript::Close()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
		// 벡터 순회하면서 변경해주기
		for (GameObject* weapon : mWeapon)
		{
			weapon->SetObjectState(GameObject::eObjectState::Inactive);
		}
		for (GameObject* shiled : mShiled)
		{
			shiled->SetObjectState(GameObject::eObjectState::Inactive);
		}
		for (GameObject* accessory : mAccessory)
		{
			accessory->SetObjectState(GameObject::eObjectState::Inactive);
		}
		for (GameObject* item : mItem)
		{
			item->SetObjectState(GameObject::eObjectState::Inactive);
		}
	}
	void InventoryScript::SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeSlotTexture(first);
	}
	void InventoryScript::ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeSlotTexture(texture);
	}
}