#include "daRigidbody.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daTransform.h"

namespace da
{
	using namespace math;
	Rigidbody::Rigidbody()
		: Component(enums::eComponentType::Rigidbody)
		, mDimensionType(eDimensionType::SecondDimension)
		, mMass(1.0f)
		, mFriction(10.0f)
		, mV2Force(Vector2::Zero)
		, mV2Velocity(Vector2::Zero)
		, mV2Accelation(Vector2::Zero)
		, mV2Gravity(Vector2(0.0f, 10.0f))
		, mV2LimitVelocity(Vector2(400.0f, 600.0f))
		, mV3Force(Vector3::Zero)
		, mV3Velocity(Vector3::Zero)
		, mV3Accelation(Vector3::Zero)
		, mUseGravity(false)
		, mIsGround(false)
	{
	}
	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Update()
	{
		// ���ӵ� ���ϱ� (A = F / M)
		// �ӵ� ���ϱ� (V = A * T * Default)
		if (eDimensionType::SecondDimension == mDimensionType)
		{
			mV2Accelation = mV2Force / mMass;
			mV2Velocity += mV2Accelation * (float)Time::DeltaTime() * 300.0f;
			v2FrictionAction();
			v2MoveAction();
			if (mUseGravity)
				v2GravityAction();
			v2LimitVelocityAction();
		}
		else
		{
			mV3Accelation = mV3Force / mMass;
			mV3Velocity += mV3Accelation * (float)Time::DeltaTime() * 300.0f;
			v3FrictionAction();
			v3MoveAction();
			if (mUseGravity)
				v3GravityAction();
			v3LimitVelocityAction();
		}
	}

	void Rigidbody::v2FrictionAction()
	{
		if (!(mV2Velocity == Vector2::Zero))
		{
			// ���� ���� ���ϱ� (�ӵ��� �ݴ� ����)
			Vector2 frictionUnit = -mV2Velocity;
			frictionUnit.Normalize();

			// ������ ũ��
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			// �̵����� �ƴ϶�� ������ ����
			if (Vector2::Zero == mV2Force)
				friction += friction;

			// �������� �ӵ��� �ѱ��
			if (friction.Length() > mV2Velocity.Length())
				mV2Velocity = Vector2::Zero;
			else
				mV2Velocity += friction;
		}
	}
	void Rigidbody::v2MoveAction()
	{
		// ���� Position ��������
		Vector3 ownerPosition = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 movePosision(ownerPosition.x, ownerPosition.y);

		// �� ��ġ�� �ӵ� �����ֱ�
		movePosision += mV2Velocity * (float)Time::DeltaTime();


		// ����� ��ġ�� �������ֱ�
		ownerPosition.x = movePosision.x;
		ownerPosition.y = movePosision.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(ownerPosition);

		// �� �ʱ�ȭ
		mV2Force = Vector2::Zero;
	}
	void Rigidbody::v2GravityAction()
	{
		if (mIsGround)
		{
			// ���� ����� �߷� �����ϱ�
			EraseVelocity(mV2Gravity, mV2Velocity);
		}
		else
		{
			// �ӵ��� �߷� ���ϱ�
			mV2Velocity += mV2Gravity * (float)Time::DeltaTime();
			// ���Ŀ� �߷� ������� �߰�
		}
	}
	void Rigidbody::v2LimitVelocityAction()
	{
		// ����, ���� �ӵ� ���� �и�
		Vector2 gravityUnit = mV2Gravity;
		gravityUnit.Normalize();

		// ���� �ӵ� ���ϱ�
		float dot = gravityUnit.Dot(mV2Gravity);
		Vector2 verticalVelocity = gravityUnit;
		verticalVelocity *= dot;

		// ���� �ӵ� ���ϱ�
		Vector2 horizontalVelocity = mV2Velocity - verticalVelocity;


		// �� ������ �и��ؼ� ��� ����
		if (verticalVelocity.Length() > mV2LimitVelocity.y)
		{
			verticalVelocity.Normalize();
			verticalVelocity *= mV2LimitVelocity.y;
		}
		if (horizontalVelocity.Length() > mV2LimitVelocity.x)
		{
			horizontalVelocity.Normalize();
			horizontalVelocity *= mV2LimitVelocity.x;
		}

		// �ٽ� ����, ���� �ӵ��� ���ļ� ����
		mV2Velocity = verticalVelocity + horizontalVelocity;
	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// ������� ������ �������� ���ϱ�
		Vector2 unit = dir;
		unit.Normalize();

		// �������Ϳ� �ӷ��� �����ؼ� ������ �ϴ� �ӷ��� ����
		float dot = unit.Dot(velocity);

		// ���� �ӵ����� Ư�� ������ ���͸� ������ 
		mV2Velocity -= unit * dot;
	}
}
