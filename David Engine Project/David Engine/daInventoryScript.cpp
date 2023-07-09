#include "daInventoryScript.h"

#include "daGameObject.h"

namespace da
{
	InventoryScript::InventoryScript()
	{
	}
	InventoryScript::~InventoryScript()
	{
	}
	void InventoryScript::Update()
	{
	}
	void InventoryScript::Open()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Active);
	}
	void InventoryScript::Close()
	{
		GetOwner()->SetObjectState(GameObject::eObjectState::Hide);
	}
}