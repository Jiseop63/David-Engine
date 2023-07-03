#pragma once
#include "daScript.h"

namespace da
{
	class MoveShaderScript : public Script
	{
	public:
		MoveShaderScript();
		virtual ~MoveShaderScript();

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
