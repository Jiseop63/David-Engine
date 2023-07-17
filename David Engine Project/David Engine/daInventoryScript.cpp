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
		// 인벤토리
		if (Input::GetKeyDown(eKeyCode::V))
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
		if (Input::GetKeyDown(eKeyCode::TILDE))
		{
			if (true == mSelectLeft)
			{
				ChangeTexture(mSecondTexture);
				mSelectLeft = false;
			}
			else
			{
				ChangeTexture(mFirstTexture);
				mSelectLeft = true;
			}
		}
	}
	void InventoryScript::Open()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void InventoryScript::Close()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
	}
	void InventoryScript::SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeTexture(first);
	}
	void InventoryScript::ChangeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
	}
}