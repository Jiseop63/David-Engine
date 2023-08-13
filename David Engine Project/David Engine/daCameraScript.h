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

		void moveCamera();
		void debugInputCamera();

		void SetCamera(Camera* component) { mCameraComponent = component; }

		void CameraShake();
		// ��鸮������, ��鸲 �����ð�
		void SetOscillation(float oscillationPower, float time)
		{
			mCameraShaking = true;
			mOscillationPower = oscillationPower;
			mShakeValidTime = time;
		}


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