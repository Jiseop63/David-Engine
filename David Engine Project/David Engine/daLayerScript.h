#pragma once
#include "daScript.h"
#include "daCamera.h"

namespace da
{
	class LayerScript : public Script
	{
	public:
		LayerScript();
		virtual ~LayerScript();

		virtual void LateUpdate();

		void SetCamera(Camera* component) { mCamera = component; }

	private:
		Camera* mCamera;
	};

}