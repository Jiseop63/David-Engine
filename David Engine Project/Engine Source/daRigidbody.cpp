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
		
		/*ApplyGravity();
		CalculateAcceleration();
		ApplyLimitVelocity();
		ApplyFriction();
		ApplyLocation();

		mForce = Vector2::Zero;
		mAcceleration = Vector2::Zero;*/


		// �߷� ���� ����
		// ���ڷ� ���� ���� �ӽ� velocity�� ����
		// ������ ����
		// ����� �Ϸ�� �ӽ� velocity�� clamp�� ���� ����
		// ���� ����� ����
		// �ʱ�ȭ

		mTempVelocity = mTotalVelocity;

		applyGravity();
		calculateInput();
		applyFriction();
		calculateVelocity();
		applyLocation();
		clearPower();
	}

	void Rigidbody::CalculateAcceleration()
	{
		// ���ӵ� ���ϱ� (A = F / M)
		mAcceleration = mForce / mMass;
		//std::clamp();
		// ���� ���� �߰��غ�����

		// 1. ���߿��� �����̴� �� ����
		// Ground �� false�� ���, move�� ������ force�� ����
		
		// 2. �⺻���� �ӵ� ���� ���� (Clamp)
		// Move�� ����Ǵ� ���ӵ��� �Ѱ踦 �α�
		// Jump, Dash ���� �ӵ� �������ֱ�..?




		// force clamp
		// force�� �� ���� �׻� ������
		// force�� velocity�� DT������ ������
		// �������� force�� friction���� ũ�� ������, ���� �������� ������



		// velocity clamp




		mVelocity += mAcceleration * (float)Time::DeltaTime() * 1.50f;
	}

	void Rigidbody::ApplyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (footCollider->IsGround())
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
		float limitVelocityMagnitude = 7.50f;

		float totalVelocity = mVelocity.Length();

		if (totalVelocity > limitVelocityMagnitude)
		{
			mVelocity.Normalize();
			mVelocity *= limitVelocityMagnitude;
		}

		// �ӵ����� ���������� �����ϱ�
		// Jump Limit
		// Dash Limit
		// Move Limit
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
			{
				Collider2D* footCollider = GetOwner()->GetFootCollider();

				if (footCollider->IsGround())
					friction *= 12.0f;

			}

			// �⺻ ������
			// if (mVelocity.LengthSquared() < friction.LengthSquared())
			// 	mVelocity = Vector2::Zero;
			// else
			// 	mVelocity += friction;


			// ������ΰ�� x������ ���̱�
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

	void Rigidbody::applyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (footCollider->IsGround())
		{
			// ���� �ӵ��� y���� �����ϱ�
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mTempVelocity -= gravity * dot;
		}
		else
		{
			// �ӽ� velocity�� �߷� ����
			mTempVelocity += gravity * (float)Time::DeltaTime();
		}
	}

	void Rigidbody::calculateInput()
	{
		mAccelerationDir = mForceDir / mMass;
		
		mTempVelocity += mAccelerationDir * (float)Time::DeltaTime() * 1.50f;
		mTempVelocity += mVelocityDir;
	}

	void Rigidbody::applyFriction()
	{
		// ����ӵ��� ������ ���� ������ ���
		if (0 < mTempVelocity.LengthSquared())
		{
			// ������(���) ����ϱ�
			Vector2 frictionUnit = -mTempVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			// �߰� �Է��� ����, ���� ����ִ� ��쿡�� ������ ���� ������
			if (0 >= mForceDir.LengthSquared())
			{
				Collider2D* footCollider = GetOwner()->GetFootCollider();

				if (footCollider->IsGround())
					friction *= 12.0f;
			}

			// �������� ����ӵ����� ū���
			if (abs(mTempVelocity.x) < abs(friction.x))
			{
				mTempVelocity.x = 0.0f;
			}
			else
			{
				mTempVelocity.x += friction.x;
			}
		}
	}

	void Rigidbody::calculateVelocity()
	{	
		
		// clamp ���ֱ�
		float velocityMagnitude = mTempVelocity.Length();
		mTempVelocity.Normalize();
		Vector2 calcVelocity;
				
		// ApplyForce ��� -> ClampForce ����
		if (0 < mForceDir.LengthSquared())
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -mClampForce / 2.0f, mClampForce / 2.0f);
		}
		// ApplyVelocity ��� -> ClampVelocity ����
		else if (0 < mVelocityDir.LengthSquared())
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -mClampVelocity / 2.0f, mClampVelocity / 2.0f);
		}
		// �Ѵ� �ƴϸ� �׳� ��� ��
		else
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -10.0f, 10.0f);
		}

		// �׳� y�� Ŭ����
		// x�� Ŭ���� ���� �ΰ�
		// Dash clamp�� ���� �δ°� ��������



		// Clamp ����� �ӵ��� ������
		mTotalVelocity = calcVelocity;
	}

	void Rigidbody::applyLocation()
	{
		// ���� Position ��������
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// �� ��ġ�� �ӵ� �����ֱ�
		calcPos += mTotalVelocity * (float)Time::DeltaTime();

		// ����� ��ġ�� �������ֱ�
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);
	}
	void Rigidbody::clearPower()
	{
		mTempVelocity = Vector2::Zero;
		mAccelerationDir = Vector2::Zero;
		mForceDir = Vector2::Zero;
		mVelocityDir = Vector2::Zero;
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
