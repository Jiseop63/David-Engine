#pragma once
#include "daScript.h"

namespace da
{
	class DashCountScript : public Script
	{
	public:
		DashCountScript ();
		virtual ~DashCountScript ();

		virtual void LateUpdate();
		void BindConstantBuffer();
	};
}