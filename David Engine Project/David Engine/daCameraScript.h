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

		virtual void Update() override;

	public:
		void SetCamera(Camera* component) { mCameraComponent = component; }
		// ��鸮������, ��鸲 �����ð�
		void SetOscillation(float oscillationPower, float time)
		{
			mCameraShaking = true;
			mOscillationPower = oscillationPower;
			mShakeValidTime = time;
		}
	private:
		void moveCamera();
		void cameraShake();



	private:
		void debugInputCamera();
	public:
		Camera* mCameraComponent;

	private:
		math::Vector3 mOriginPosition;
		bool	mCameraShaking;
		float	mOscillationPower;
		float	mShakeAccumulateTime;
		float	mShakeValidTime;
	};
}