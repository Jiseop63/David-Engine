#include "daRigidbody.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daCollider2D.h"

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
		mFriction = 4.50f;
	}
	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Update()
	{
		
		CalculateAcceleration();

		ApplyGravity();
		ApplyLimitVelocity();
		ApplyFriction();					
		ApplyLocation();

		// �� �ʱ�ȭ
		mForce = Vector2::Zero;
		mAcceleration = Vector2::Zero;
	}

	void Rigidbody::CalculateAcceleration()
	{
		// ���ӵ� ���ϱ� (A = F / M)
		mAcceleration = mForce / mMass;
		mVelocity += mAcceleration * (float)Time::DeltaTime() * 1.50f;
	}

	void Rigidbody::ApplyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		bool isGround = footCollider->IsGround();		
		if (isGround)
		{
			// y �ӵ� �����ϱ�
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;

			// ���� �İ������� ���� �÷��ֱ�
			// �̰� ������ �ϴ°� ������
		}
		else
		{
			// �Ʒ��� �� ����
			mVelocity += gravity * (float)Time::DeltaTime();
		}
	}

	void Rigidbody::ApplyLimitVelocity()
	{
		float limitVelocityMagnitude = 3.50f;

		float totalVelocity = mVelocity.Length();

		if (totalVelocity > limitVelocityMagnitude)
		{
			mVelocity.Normalize();
			mVelocity *= limitVelocityMagnitude;
		}
	}

	void Rigidbody::ApplyFriction()
	{
		if (0 < mVelocity.LengthSquared())
		{
			Vector2 frictionUnit = -mVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();


			// �߰� �Է��� ������ ������ n��
			if (0 >= mForce.LengthSquared())
				friction *= 12.0f;

			// �⺻ ������
			// if (mVelocity.LengthSquared() < friction.LengthSquared())
			// 	mVelocity = Vector2::Zero;
			// else
			// 	mVelocity += friction;
			if (abs(mVelocity.x) < abs(friction.x))
				mVelocity.x = 0.0f;
			else
				mVelocity.x += friction.x;
		}
	}

	void Rigidbody::ApplyLocation()
	{
		// ���� Position ��������
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// �� ��ġ�� �ӵ� �����ֱ�
		calcPos += mVelocity * (float)Time::DeltaTime();

		// ����� ��ġ�� �������ֱ�
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);
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
