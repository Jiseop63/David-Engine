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
		, mMass(0.0f)
		, mFriction(0.0f)
		, mForce(Vector2::Zero)
		, mAcceleration(Vector2::Zero)
		, mVelocity(Vector2::Zero)		
	{
		mMass = 1.0f;
		mFriction = 3.0f;
	}
	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Update()
	{
		// ���ӵ� ���ϱ� (A = F / M)
		mAcceleration = mForce / mMass;
		mVelocity += mAcceleration * (float)Time::DeltaTime() * 2.0f;

		// gravity
		Vector2 gravity(0.0f, 0.980f);


		// limit
		float limitMagnitude = 2.5f;

		float totalVelocity = mVelocity.Length();

		if (totalVelocity > limitMagnitude)
		{
			mVelocity.Normalize();
			mVelocity *= limitMagnitude;
		}


		// friction
		if (0 < mVelocity.LengthSquared())
		{
			Vector2 frictionUnit = -mVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();
			
			
			// �߰� �Է��� ������ ������ 8��
			if (0 >= mForce.LengthSquared())
				friction *= 8.0f;
			
			// �⺻ ������
			if (mVelocity.LengthSquared() < friction.LengthSquared())
				mVelocity = Vector2::Zero;
			else
				mVelocity += friction;
			
		}

		// ���� Position ��������
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// �� ��ġ�� �ӵ� �����ֱ�
		calcPos += mVelocity * (float)Time::DeltaTime();

		// ����� ��ġ�� �������ֱ�
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);

		// �� �ʱ�ȭ
		mForce = Vector2::Zero;

	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// ������� ������ �������� ���ϱ�
		Vector2 unit = dir;
		unit.Normalize();

		// �������Ϳ� �ӷ��� �����ؼ� ������ �ϴ� �ӷ��� ����
		float dot = unit.Dot(velocity);

		// ���� �ӵ����� Ư�� ������ ���͸� ������ 
		mVelocity -= unit * dot;
	}
}
