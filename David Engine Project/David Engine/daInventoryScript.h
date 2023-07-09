#pragma once
#include "daScript.h"

namespace da
{
	class InventoryScript : public Script
	{
	public:
		InventoryScript();
		virtual ~InventoryScript();

		virtual void Update();

	protected:
		bool mFocusOn;
	};
}
