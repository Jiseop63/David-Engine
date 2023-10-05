#include "daItemSlotScript.h"

#include "daInput.h"
#include "daGameObject.h"

namespace da
{
	using namespace math;

	ItemSlotScript::ItemSlotScript()
		: mSlotTransform(nullptr)
		, mSlotRenderer(nullptr)
	{
	}
	ItemSlotScript::~ItemSlotScript()
	{		
	}
	void ItemSlotScript::Initialize()
	{
		mSlotTransform = GetOwner()->GetTransform();
		mSlotRenderer = GetOwner()->GetComponent<MeshRenderer>();
	}
	void ItemSlotScript::ClearPosition()
	{
		math::Vector3 slotPosition = mSlotTransform->GetPosition();
		// �� �̺κп��� ���װ� �����
		mItemScript->SetItemPosition(slotPosition);
	}
}