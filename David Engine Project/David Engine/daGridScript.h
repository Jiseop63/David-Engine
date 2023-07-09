#pragma once
#include "daScript.h"
#include "daCamera.h"

namespace da
{
	class GridScript : public Script
	{
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		void SetCamera(Camera* component) { mCamera = component; }

	private:
		Camera* mCamera;
	};
}
