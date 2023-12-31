#include "daRigidbody.h"
#include "daTime.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daCollider2D.h"
#include "daInput.h"
#include "daSceneManager.h"

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
		, mGravityAble(true)
		, mInputForceMagnitude(0.0f)
		, mInputVelocityMagnitude(0.0f)
		, mAccelerationMagnitude(0.0f)
		, mVelocityMagnitude(0.0f)
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
		if (!mUsing)
			return;
		mCalcVelocity = mPreviousVelocity;
		applyGravity();
		calculateVelocity();
		applyFriction();
		applyLocation();
		clearPower();
	}

	void Rigidbody::applyGravity()
	{
		Vector2 gravity(0.0f, -14.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (!footCollider)
			return;

		if (footCollider->IsGround())
		{			
			// y 속도 제거하기
			gravity.Normalize();
			float dot = mCalcVelocity.Dot(gravity);
			mCalcVelocity -= gravity * dot;
		}
		else
		{
			if (mGravityAble)
			{
				// 아래로 힘 적용
				mCalcVelocity += gravity * (float)Time::DeltaTime();
				mCalcVelocity.y = std::clamp(mCalcVelocity.y, -mLimitGravityForce, mLimitGravityForce);
			}
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

		// 가속도 적용
		mCalcVelocity += mAccumulateAcceleration;

		// 속도 적용
		if (0 >= mOverrideVelocity.LengthSquared())
			mCalcVelocity += mAccumulateVelocity;
		else
		{
			mCalcVelocity = mOverrideVelocity;
			mOverrideVelocity = Vector2::Zero;
		}
		
		wallCollision();
	}

	void Rigidbody::wallCollision()
	{
		// 주체인 충돌체를 가져옴
		Collider2D* bodyCollider = GetOwner()->GetBodyCollider();

		// 벽에 충돌일 경우에만 진행
		if (Collider2D::eWallCollisionState::None == bodyCollider->IsWallCollision())
			return;

		switch (bodyCollider->IsWallCollision())
		{
		case da::Collider2D::eWallCollisionState::Left:
		{
			if (0 >= mCalcVelocity.x)
				mCalcVelocity.x = 0.10f;
		}
			break;
		case da::Collider2D::eWallCollisionState::Right:
		{
			if (0 <= mCalcVelocity.x)
				mCalcVelocity.x = -0.10f;
		}
			break;
		case da::Collider2D::eWallCollisionState::Top:
		{
			// 아래로 살작 밀기
			if (0 <= mCalcVelocity.y)
				mCalcVelocity.y = -0.10f;
		}
			break;
		default:
			break;
		}
	}

	void Rigidbody::applyFriction()
	{
		// 현재속도가 멈추지 않은 상태인 경우
		if (0 < mCalcVelocity.LengthSquared())
		{
			// 마찰력(상수) 계산하기
			Vector2 frictionUnit = -mCalcVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			
			// 마찰력 증가 조건
			// 1. 키입력을 안받음
			// 2. 지상에 있음
			Collider2D* footCollider = GetOwner()->GetFootCollider();
			if (footCollider->IsGround())
			{
				friction *= 30.0f;
			}

			// 마찰력이 현재속도보다 큰경우
			if (abs(mCalcVelocity.x) < abs(friction.x))
				mCalcVelocity.x = 0.0f;
			else
				mCalcVelocity.x += friction.x;
		
		}
	}
	void Rigidbody::applyLocation()
	{
		// 오너 Position 가져오기
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// 내 위치에 속도 더해주기
		calcPos += mCalcVelocity * (float)Time::DeltaTime();

		// 변경된 위치를 갱신해주기
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);
	}
	void Rigidbody::clearPower()
	{
		// 계산된 속도 저장하기
		mPreviousVelocity = mCalcVelocity;
		// 임시 속도 제거
		mCalcVelocity = Vector2::Zero;
		
	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// 지우려는 방향의 단위벡터 구하기
		Vector2 unit = dir;
		unit.Normalize();

		// 단위벡터에 속력을 내적해서 지워야 하는 속력을 구함
		float dot = unit.Dot(velocity);

		// 현재 속도에서 특정 방향의 벡터를 지워줌 
		mPreviousVelocity -= unit * dot;
	}
}
