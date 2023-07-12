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
		, mFriction(5.0f)
		, mV2Force(Vector2::Zero)
		, mV2Velocity(Vector2::Zero)
		, mV2Accelation(Vector2::Zero)
		, mV2Gravity(Vector2(0.0f, 10.0f))
		, mV2LimitVelocity(Vector2(12.0f, 98.0f))
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
		// 가속도 구하기 (A = F / M)
		// 속도 구하기 (V = A * T * Default)
		if (eDimensionType::SecondDimension == mDimensionType)
		{
			mV2Accelation = mV2Force / mMass;
			mV2Velocity += mV2Accelation * (float)Time::DeltaTime() * 1.60f;
			v2FrictionAction();
			v2MoveAction();
			if (mUseGravity)
				v2GravityAction();
			v2LimitVelocityAction();
		}
		else
		{
			mV3Accelation = mV3Force / mMass;
			mV3Velocity += mV3Accelation * (float)Time::DeltaTime();
			v3FrictionAction();
			v3MoveAction();
			if (mUseGravity)
				v3GravityAction();
			v3LimitVelocityAction();
		}
	}

	void Rigidbody::v2FrictionAction()
	{
		// 이동중이라면
		if (!(mV2Velocity == Vector2::Zero))
		{
			// 마찰 방향 구하기 (속도의 반대 방향)
			Vector2 frictionUnit = -mV2Velocity;
			frictionUnit.Normalize();

			// 마찰력 크기
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			// 이동중이 아니라면 마찰력 증가
			//if (Vector2::Zero == mV2Force)
				//friction += friction;

			// 마찰력이 속도를 넘기면
			if (friction.Length() > mV2Velocity.Length())
				mV2Velocity = Vector2::Zero;
			else
				mV2Velocity += friction;
		}
	}
	void Rigidbody::v2MoveAction()
	{
		// 오너 Position 가져오기
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// 내 위치에 속도 더해주기
		calcPos += mV2Velocity * (float)Time::DeltaTime();


		// 변경된 위치를 갱신해주기
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);

		// 힘 초기화
		mV2Force = Vector2::Zero;
	}
	void Rigidbody::v2GravityAction()
	{
		if (mIsGround)
		{
			// 현재 적용된 중력 제거하기
			EraseVelocity(mV2Gravity, mV2Velocity);
		}
		else
		{
			// 속도에 중력 더하기
			mV2Velocity += mV2Gravity * (float)Time::DeltaTime();
			// 추후에 중력 조절기능 추가
		}
	}
	void Rigidbody::v2LimitVelocityAction()
	{
		// 수직, 수평 속도 성분 분리
		Vector2 gravityUnit = mV2Gravity;
		gravityUnit.Normalize();

		// 수직 속도 구하기
		float dot = gravityUnit.Dot(mV2Gravity);
		Vector2 verticalVelocity = gravityUnit;
		verticalVelocity *= dot;

		// 수평 속도 구하기
		Vector2 horizontalVelocity = mV2Velocity - verticalVelocity;


		// 각 성분을 분리해서 계산 적용
		if (verticalVelocity.Length() > mV2LimitVelocity.y)
		{
			verticalVelocity.Normalize();
			verticalVelocity.y = mV2LimitVelocity.y;
		}
		if (horizontalVelocity.Length() > mV2LimitVelocity.x)
		{
			horizontalVelocity.Normalize();
			horizontalVelocity.x = mV2LimitVelocity.x;
		}

		// 다시 수평, 수직 속도를 합쳐서 갱신
		mV2Velocity = verticalVelocity + horizontalVelocity;
	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// 지우려는 방향의 단위벡터 구하기
		Vector2 unit = dir;
		unit.Normalize();

		// 단위벡터에 속력을 내적해서 지워야 하는 속력을 구함
		float dot = unit.Dot(velocity);

		// 현재 속도에서 특정 방향의 벡터를 지워줌 
		mV2Velocity -= unit * dot;
	}
}
