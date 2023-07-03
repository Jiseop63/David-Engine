#pragma once
#include "daComponent.h"

namespace da
{
	class Rigidbody : public Component
	{
		enum eDimensionType
		{
			SecondDimension,
			ThirdDimension,
		};

		Rigidbody();
		virtual ~Rigidbody();

		virtual void Initialize() {};
		virtual void Update();
		virtual void LateUpdate() {};
		virtual void Render() {};

	public:
		void SetDimentionType(eDimensionType dimension) { mDimensionType = dimension; }

		// unit : 속도 통제 방향, inner : 현재 속도
		void EraseVelocity(math::Vector2 dir, math::Vector2 velocity);
		void EraseVelocity(math::Vector3 dir, math::Vector3 velocity) {};

	private:
		void v2FrictionAction();
		void v2MoveAction();
		void v2GravityAction();
		void v2LimitVelocityAction();
	private:
		void v3FrictionAction() {};
		void v3MoveAction() {};
		void v3GravityAction() {};
		void v3LimitVelocityAction() {};


	private:
		eDimensionType	mDimensionType;
		float			mMass;			// 질량
		float			mFriction;		// 마찰력

		// V2
		math::Vector2	mV2Force;			// 가해지는 힘
		math::Vector2	mV2Velocity;		// 속도
		math::Vector2	mV2Accelation;		// 가속도
		math::Vector2	mV2LimitVelocity;	// 최대 제한속도
		math::Vector2	mV2Gravity;			// 중력 속도

		// V3
		math::Vector3	mV3Force;			// 가해지는 힘
		math::Vector3	mV3Velocity;		// 속도
		math::Vector3	mV3Accelation;		// 가속도
		//math::Vector3	mV3LimitVelocity;	// 최대 제한속도
		//math::Vector3	mV3Gravity;			// 중력 속도
		bool			mIsGround;
		bool			mUseGravity;
	};
}