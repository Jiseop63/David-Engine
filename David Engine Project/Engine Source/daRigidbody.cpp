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
		, mForceMagnitude(0.0f)
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
		
		CalculateVelocity();

		ApplyGravity();
		ApplyLimitVelocity();
		ApplyFriction();					
		ApplyLocation();

		// �� �ʱ�ȭ
		mForce = Vector2::Zero;
		mAcceleration = Vector2::Zero;
		mForceMagnitude = 0.0f;
	}

	void Rigidbody::CalculateVelocity()
	{
		// ���ӵ� ���ϱ� (A = F / M)
		mAcceleration = mForce * mForceMagnitude / mMass;
		mVelocity += mAcceleration * (float)Time::DeltaTime() * 3.0f;
	}

	void Rigidbody::ApplyGravity()
	{
		std::vector<Collider2D*> onwerColliders = GetOwner()->GetComponents<Collider2D>();

		Collider2D* footCollider = nullptr;
		for (Collider2D* collider : onwerColliders)
		{
			if (Collider2D::eColliderDetection::Land == collider->GetColliderDetection())
				footCollider = collider;
		}

		if (nullptr == footCollider)
		{
			int a = 0;
			return;
		}
		bool isGround = footCollider->IsGround();

		Vector2 gravity(0.0f, 0.980f);
		
		if (isGround)
		{
			// y �ӵ� �����ϱ�
			gravity.Normalize();
			// ���� �İ������� ���� �÷��ֱ�
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			// �Ʒ��� �� ����
			mVelocity += gravity * Time::DeltaTime();
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
			if (mVelocity.LengthSquared() < friction.LengthSquared())
				mVelocity = Vector2::Zero;
			else
				mVelocity += friction;
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
