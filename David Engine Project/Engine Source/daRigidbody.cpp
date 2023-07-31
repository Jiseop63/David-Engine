#include "daRigidbody.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daCollider2D.h"
#include "daInput.h"
namespace da
{
	using namespace math;
	Rigidbody::Rigidbody()
		: Component(enums::eComponentType::Rigidbody)
		, mDimensionType(eDimensionType::SecondDimension)
		, mMass(0.0f)
		, mFriction(0.0f)
		, mPreviousVelocity(Vector2::Zero)
		, mCalcVelocity(Vector2::Zero)
		, mAccumulateAcceleration(Vector2::Zero)
		, mAccumulateVelocity(Vector2::Zero)
		, mMaxMagnitudeForce(0.0f)
		, mMaxMagnitudeVelocity(0.0f)
		, mLimitGravityForce(0.0f)
		, mMoving(false)
	{
		mMass = 1.0f;
		mFriction = 1.0f;

		mMaxMagnitudeForce = 1.0f;
		mMaxMagnitudeVelocity = 1.0f;
		mLimitGravityForce = 9.80f;
	}
	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Update()
	{		
		mCalcVelocity = mPreviousVelocity;
		applyGravity();
		calculateVelocity();
		applyFriction();
		applyLocation();
		clearPower();
	}

	void Rigidbody::applyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (footCollider->IsGround())
		{
			// y �ӵ� �����ϱ�
			gravity.Normalize();
			float dot = mCalcVelocity.Dot(gravity);
			mCalcVelocity -= gravity * dot;
		}
		else
		{
			// �Ʒ��� �� ����
			mCalcVelocity += gravity * (float)Time::DeltaTime();
			mCalcVelocity.y = std::clamp(mCalcVelocity.y, -mLimitGravityForce, mLimitGravityForce);
		}
	}

	
	void Rigidbody::calculateVelocity()
	{	
		// InputForce
		mAccelerationMagnitude = mInputForceMagnitude / mMass;
		mAccumulateAcceleration = mInputForceDir * mAccelerationMagnitude * (float)Time::DeltaTime();
		mInputForceDir = Vector2::Zero;
		mInputForceMagnitude = 0.0f;

		// InputVelocity
		mAccumulateVelocity = mInputVelocityDir * mInputVelocityMagnitude;
		mInputVelocityDir = Vector2::Zero;
		mInputVelocityMagnitude = 0.0f;

		// ���ӵ� ����
		mCalcVelocity += mAccumulateAcceleration;

		// �ӵ� ����
		if (0 >= mOverrideVelocity.LengthSquared())
			mCalcVelocity += mAccumulateVelocity;
		else
		{
			mCalcVelocity = mOverrideVelocity;
			mOverrideVelocity = Vector2::Zero;
		}
	}

	void Rigidbody::applyFriction()
	{
		// ����ӵ��� ������ ���� ������ ���
		if (0 < mCalcVelocity.LengthSquared())
		{
			// ������(���) ����ϱ�
			Vector2 frictionUnit = -mCalcVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			
			// ������ ���� ����
			// 1. Ű�Է��� �ȹ���
			// 2. ���� ����
			Collider2D* footCollider = GetOwner()->GetFootCollider();
			if (footCollider->IsGround())
			{
				friction *= 30.0f;
				/*if ((0 < mCalcVelocity.x && 0 > mInputForceDir.x)
					|| (0 > mCalcVelocity.x && 0 < mInputForceDir.x)
					|| false == mMoving)*/
					
			}
			
			// �������� ����ӵ����� ū���
			if (abs(mCalcVelocity.x) < abs(friction.x))
				mCalcVelocity.x = 0.0f;
			else
				mCalcVelocity.x += friction.x;
		}
	}
	void Rigidbody::applyLocation()
	{
		// ���� Position ��������
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// �� ��ġ�� �ӵ� �����ֱ�
		calcPos += mCalcVelocity * (float)Time::DeltaTime();

		// ����� ��ġ�� �������ֱ�
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);
	}
	void Rigidbody::clearPower()
	{
		// ���� �ӵ� �����ϱ�
		mPreviousVelocity = mCalcVelocity;
		// �ӽ� �ӵ� ����
		mCalcVelocity = Vector2::Zero;
		
	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// ������� ������ �������� ���ϱ�
		Vector2 unit = dir;
		unit.Normalize();

		// �������Ϳ� �ӷ��� �����ؼ� ������ �ϴ� �ӷ��� ����
		float dot = unit.Dot(velocity);

		// ���� �ӵ����� Ư�� ������ ���͸� ������ 
		mPreviousVelocity -= unit * dot;
	}
}
