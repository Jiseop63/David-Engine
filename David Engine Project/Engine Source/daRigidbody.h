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
		void CalculateVelocity();
		void ApplyGravity();
		void ApplyLimitVelocity();
		void ApplyFriction();
		void ApplyLocation();
	public:
		void SetDimentionType(eDimensionType dimension) { mDimensionType = dimension; }

		void ApplyForce(math::Vector2 vector2, float magnitude) 
		{ 
			mForce = vector2; 
			mForceMagnitude = magnitude;
		}
		math::Vector2 GetVelocity() { return mVelocity; }
		void ApplyVelocity(math::Vector2 vector2) { mVelocity = vector2; }

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
		float			mForceMagnitude;

	private:
		math::Vector2 mForce;
		math::Vector2 mAcceleration;
		math::Vector2 mVelocity;
	};
}