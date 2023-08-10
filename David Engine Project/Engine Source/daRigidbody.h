#pragma once
#include "daComponent.h"

namespace da
{
	class Rigidbody : public Component
	{
	public:
		enum eDimensionType
		{
			SecondDimension,
			ThirdDimension,
		};

		Rigidbody();
		virtual ~Rigidbody();

		virtual void Update();

	public:
		void applyGravity();

		void calculateVelocity();
		void applyFriction();
		void applyLocation();
		void clearPower();

	public:
		void SetDimentionType(eDimensionType dimension) { mDimensionType = dimension; }
		void SetMoving(bool isMove) { mMoving = isMove; }
		void GravityAble(bool value) { mGravityAble = value; }
	public:
		void ApplyForce(math::Vector2 dir, float magnitude) { mInputForceDir = dir; mInputForceMagnitude = magnitude; }
		void ApplyVelocity(math::Vector2 dir, float magnitude) { mInputVelocityDir = dir; mInputVelocityMagnitude = magnitude; }
		void OverrideVelocity(math::Vector2 dir, float magnitude) { mOverrideVelocity = dir * magnitude; }
		math::Vector2 GetVelocity() { return mPreviousVelocity; }

		// unit : 속도 통제 방향, inner : 현재 속도
		void EraseVelocity(math::Vector2 dir, math::Vector2 velocity);


	private:
		eDimensionType	mDimensionType;
		float			mMass;			// 질량
		float			mFriction;		// 마찰력

	private:
		math::Vector2	mPreviousVelocity;
		math::Vector2	mCalcVelocity;
		// input
		math::Vector2	mInputForceDir;
		math::Vector2	mInputVelocityDir;
		math::Vector2	mOverrideVelocity;

		float			mInputForceMagnitude;
		float			mInputVelocityMagnitude;

	private:
		math::Vector2	mAccumulateAcceleration;
		math::Vector2	mAccumulateVelocity;




		float			mLimitGravityForce;
		float			mAccelerationMagnitude;
		float			mMaxMagnitudeForce;
		float			mVelocityMagnitude;
		float			mMaxMagnitudeVelocity;
		bool			mMoving;
		bool			mGravityAble;
	};
}