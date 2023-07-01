#pragma once
#include "daGameObject.h"

namespace da
{
	class Camera;
	class CameraObject : public GameObject
	{
	public:
		CameraObject();
		virtual ~CameraObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		Camera* GetCameraComponent() { return mCameraComponent; }
	private:
		Camera* mCameraComponent;
	};
}