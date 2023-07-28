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
		void CalculateAcceleration();
		void ApplyGravity();
		void ApplyLimitVelocity();
		void ApplyFriction();
		void ApplyLocation();



		void applyGravity();
		void calculateInput();
		void applyFriction();
		void calculateVelocity();
		void applyLocation();
		void clearPower();

	public:
		void SetDimentionType(eDimensionType dimension) { mDimensionType = dimension; }

		void ApplyForce(math::Vector2 vector2) { mForce = vector2; }
		math::Vector2 GetVelocity() { return mVelocity; }
		void ApplyVelocity(math::Vector2 vector2) { mVelocity += vector2; }


	public:
		void ApplyForce(math::Vector2 dir, float magnitude) { mForceDir = dir * magnitude; mClampForce = magnitude; }
		void ApplyVelocity(math::Vector2 dir, float magnitude) { mVelocityDir = dir * magnitude; mClampVelocity = magnitude; }
		// unit : 속도 통제 방향, inner : 현재 속도
		void EraseVelocity(math::Vector2 dir, math::Vector2 velocity);

	private:
		void FrictionAction() {}
		void MoveAction() {}
		void GravityAction() {}
		void LimitVelocityAction() {}

	private:
		eDimensionType	mDimensionType;
		float			mMass;			// 질량
		float			mFriction;		// 마찰력

	private:
		math::Vector2 mForce;
		math::Vector2 mAcceleration;
		math::Vector2 mVelocity;

	private:
		math::Vector2	mTempVelocity;
		math::Vector2	mAccelerationDir;
		math::Vector2	mForceDir;
		math::Vector2	mVelocityDir;

		float			mClampForce;
		float			mClampVelocity;

		math::Vector2	mTotalVelocity;
	};
}