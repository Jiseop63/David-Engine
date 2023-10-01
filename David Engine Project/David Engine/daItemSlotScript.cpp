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
}