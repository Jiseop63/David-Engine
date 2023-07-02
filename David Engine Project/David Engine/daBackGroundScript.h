#pragma once
#include "daScript.h"

namespace da
{
	class BackGroundScript : public Script
	{
	public:
		BackGroundScript();
		virtual ~BackGroundScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		void SetSpeed(float value) { mSpeed = value; }
	private:
		float mAddTime;
		float mSpeed;
	};
}
