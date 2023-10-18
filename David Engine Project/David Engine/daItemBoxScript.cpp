#include "daItemBoxScript.h"
#include "daInputIconScript.h"
#include "daGameObject.h"
#include "daInput.h"
#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daItemManager.h"
#include "daResources.h"
namespace da
{
	ItemBoxScript::ItemBoxScript()
		: mBoxTransform(nullptr)
		, mBoxCollider(nullptr)
		, mInputIcon(nullptr)
		, mFocused(false)
	{
	}
	ItemBoxScript::~ItemBoxScript()
	{
	}
	void ItemBoxScript::Initialize()
	{
		mBoxTransform = GetOwner()->GetTransform();
		mBoxTransform->SetScale(math::Vector3(0.230f * 4.0f, 0.160f * 4.0f, 1.0f));
		mBoxRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mBoxCollider = GetOwner()->AddComponent<Collider2D>();
		mBoxCollider->SetSize(math::Vector2(1.50f, 1.50f));
	}
	void ItemBoxScript::Update()
	{
		if (mFocused)
		{
			if (Input::GetKeyDown(eKeyCode::F))
				takeItem();
		}
	}
	void ItemBoxScript::AddIconScript(GameObject* object)
	{
		mInputIcon = object->AddComponent<InputIconScript>();
		object->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void ItemBoxScript::SetPosition(math::Vector3 position)
	{
		mBoxTransform->SetPosition(position);
		mInputIcon->SetPosition(position);
	}
	void ItemBoxScript::takeItem()
	{
		// 일단 인벤 가져오고
		InventoryScript* inventory = SceneManager::GetInventoryScript();

		// 빈칸 확인하기
		if (inventory->CheckItemSlot(enums::eItemSlot::Slot02))
			inventory->AddItemToSlot(enums::eItemSlot::Slot02, ItemManager::GetItem(mItemName));
		else if (inventory->CheckItemSlot(enums::eItemSlot::Slot20))
			inventory->AddItemToSlot(enums::eItemSlot::Slot20, ItemManager::GetItem(mItemName));
		else if (inventory->CheckItemSlot(enums::eItemSlot::Slot21))
			inventory->AddItemToSlot(enums::eItemSlot::Slot21, ItemManager::GetItem(mItemName));

		SceneManager::GetPlayerScript()->GetPlayerAudio()->Play(Resources::Find<AudioClip>(L"Equip"), 60.0f, false);
		GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
	}
	void ItemBoxScript::OnCollisionEnter(Collider2D* other)
	{
		mFocused = true;
		mInputIcon->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void ItemBoxScript::OnCollisionExit(Collider2D* other)
	{
		mFocused = false;
		mInputIcon->GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
}