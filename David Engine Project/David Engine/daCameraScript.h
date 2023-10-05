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
		// 흔들리는정도, 흔들림 유지시간
		void SetOscillation(float oscillationPower, float time)
		{
			if (mIsOscillation)
				return;
			mTurnOnShaking = true;	// 흔들리는 조건
			mIsOscillation = true;
			mOscillationPower = oscillationPower;
			mOscillationTime.End = time;
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
		bool	mTurnOnShaking;
		bool	mIsOscillation;
		float	mOscillationPower;
		structs::sActionTimeValues mOscillationTime;
		float	mShakeAccumulateTime;
		float	mShakeValidTime;
	};
}