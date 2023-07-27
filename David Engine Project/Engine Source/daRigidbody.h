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

	public:
		void SetDimentionType(eDimensionType dimension) { mDimensionType = dimension; }

		void ApplyForce(math::Vector2 vector2) { mForce = vector2; }
		math::Vector2 GetVelocity() { return mVelocity; }
		void ApplyVelocity(math::Vector2 vector2) { mVelocity += vector2; }

		// unit : �ӵ� ���� ����, inner : ���� �ӵ�
		void EraseVelocity(math::Vector2 dir, math::Vector2 velocity);

	private:
		void FrictionAction() {}
		void MoveAction() {}
		void GravityAction() {}
		void LimitVelocityAction() {}

	private:
		eDimensionType	mDimensionType;
		float			mMass;			// ����
		float			mFriction;		// ������

	private:
		math::Vector2 mForce;
		math::Vector2 mAcceleration;
		math::Vector2 mVelocity;
	};
}