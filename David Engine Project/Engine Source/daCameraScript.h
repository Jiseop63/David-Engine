#pragma once
#include "daScript.h"

namespace da
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

		virtual void Update() override;
	};
}