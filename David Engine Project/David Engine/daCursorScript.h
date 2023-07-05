#pragma once
#include "daScript.h"

namespace da
{
	class CursorScript : public Script
	{
	public:
		CursorScript();
		virtual ~CursorScript();
		virtual void LateUpdate();

	protected:
	};
}
