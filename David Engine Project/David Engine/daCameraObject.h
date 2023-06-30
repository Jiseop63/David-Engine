#pragma once
#include "daGameObject.h"

namespace da
{
	class Camera;
	class CameraScript;
	class CameraObject : public GameObject
	{
	public:
		CameraObject();
		virtual ~CameraObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		Camera* mCameraComponent;
		CameraScript* mCameraScript;
	};
}