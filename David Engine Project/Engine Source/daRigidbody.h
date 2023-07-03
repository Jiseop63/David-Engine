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

		// unit : �ӵ� ���� ����, inner : ���� �ӵ�
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
		float			mMass;			// ����
		float			mFriction;		// ������

		// V2
		math::Vector2	mV2Force;			// �������� ��
		math::Vector2	mV2Velocity;		// �ӵ�
		math::Vector2	mV2Accelation;		// ���ӵ�
		math::Vector2	mV2LimitVelocity;	// �ִ� ���Ѽӵ�
		math::Vector2	mV2Gravity;			// �߷� �ӵ�

		// V3
		math::Vector3	mV3Force;			// �������� ��
		math::Vector3	mV3Velocity;		// �ӵ�
		math::Vector3	mV3Accelation;		// ���ӵ�
		//math::Vector3	mV3LimitVelocity;	// �ִ� ���Ѽӵ�
		//math::Vector3	mV3Gravity;			// �߷� �ӵ�
		bool			mIsGround;
		bool			mUseGravity;
	};
}