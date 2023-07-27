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

		// 힘 초기화
		mForce = Vector2::Zero;
		mAcceleration = Vector2::Zero;
	}

	void Rigidbody::CalculateAcceleration()
	{
		// 가속도 구하기 (A = F / M)
		mAcceleration = mForce / mMass;

		// 조건 마구 추가해보리기

		// 1. 공중에서 움직이는 힘 감소
		// Ground 가 false인 경우, move로 들어오는 force가 감소
		
		// 2. 기본적인 속도 제한 적용 (Clamp)
		// Move로 적용되는 가속도에 한계를 두기
		// Jump, Dash 또한 속도 제한해주기..?


		mVelocity += mAcceleration * (float)Time::DeltaTime() * 1.50f;
	}

	void Rigidbody::ApplyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (footCollider->IsGround())
		{
			// y 속도 제거하기
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;

			// 땅에 파고들었으면 위로 올려주기
			// 이건 개인이 하는게 맞을듯
		}
		else
		{
			// 아래로 힘 적용
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

		// 속도제한 세부적으로 적용하기
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


			// 추가 입력이 없으면 마찰력 n배
			if (0 >= mForce.LengthSquared())
			{
				Collider2D* footCollider = GetOwner()->GetFootCollider();

				if (footCollider->IsGround())
					friction *= 12.0f;

			}

			// 기본 마찰력
			// if (mVelocity.LengthSquared() < friction.LengthSquared())
			// 	mVelocity = Vector2::Zero;
			// else
			// 	mVelocity += friction;


			// 대시중인경우 x마찰력 줄이기
			if (abs(mVelocity.x) < abs(friction.x))
				mVelocity.x = 0.0f;
			else
				mVelocity.x += friction.x;
		}
	}

	void Rigidbody::ApplyLocation()
	{
		// 오너 Position 가져오기
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// 내 위치에 속도 더해주기
		calcPos += mVelocity * (float)Time::DeltaTime();

		// 변경된 위치를 갱신해주기
		retPos.x = calcPos.x;
		retPos.y = calcPos.y;
		GetOwner()->GetComponent<Transform>()->SetPosition(retPos);
	}

	void Rigidbody::EraseVelocity(math::Vector2 dir, math::Vector2 velocity)
	{
		// 지우려는 방향의 단위벡터 구하기
		Vector2 unit = dir;
		unit.Normalize();

		// 단위벡터에 속력을 내적해서 지워야 하는 속력을 구함
		float dot = unit.Dot(velocity);

		// 현재 속도에서 특정 방향의 벡터를 지워줌 
		mVelocity -= unit * dot;
	}
}
