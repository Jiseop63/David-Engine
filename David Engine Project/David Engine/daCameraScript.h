#pragma once
#include "daScript.h"

namespace da
{
	class Camera;
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;

		void SetCamera(Camera* component) { mCameraComponent = component; }
	public:
		Camera* mCameraComponent;
	};
}