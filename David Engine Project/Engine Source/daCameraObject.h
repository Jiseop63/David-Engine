#pragma once
#include "daGameObject.h"

namespace da
{
	class Transform;
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
		Transform* mTransform;
		Camera* mCameraComponent;
		CameraScript* mCameraScript;
	};
}