#include "daEntity.h"

namespace da
{
	Entity::Entity()
		: mName(L"")
		, mID((UINT64)this)
	{
	}
	Entity::~Entity()
	{
	}
}
