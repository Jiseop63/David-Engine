#include "daItemIconScript.h"
#include "daGameObject.h"

namespace da
{
	ItemIconScript::ItemIconScript()
		: mIconTransform(nullptr)
		, mIconRenderer(nullptr)
	{
	}
	ItemIconScript::~ItemIconScript()
	{
	}
	void ItemIconScript::Initialize()
	{
		mIconTransform = GetOwner()->GetTransform();
		mIconRenderer = GetOwner()->GetComponent<MeshRenderer>();
	}
}
