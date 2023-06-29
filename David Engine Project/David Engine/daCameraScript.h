#pragma once
#include "daScript.h"

namespace da
{
	class Transform;
	class Camera;
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;

	public:
		Transform* mTransform;
		Camera* mCameraComponent;
	};
}