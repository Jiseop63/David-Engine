#include "daItemScript.h"
#include "daGameObject.h"

namespace da
{
	ItemScript::ItemScript()
	{
	}
	ItemScript::~ItemScript()
	{
	}
	void ItemScript::Initialize()
	{
		mItemTransform = GetOwner()->GetTransform();
		mItemRenderer = GetOwner()->GetComponent<MeshRenderer>();
	}
}

