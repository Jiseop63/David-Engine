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


		// 중력 값을 적용
		// 인자로 들어온 값을 임시 velocity에 적용
		// 마찰력 적용
		// 계산이 완료된 임시 velocity를 clamp를 통해 제어
		// 최종 결과물 적용
		// 초기화

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
		// 가속도 구하기 (A = F / M)
		mAcceleration = mForce / mMass;
		//std::clamp();
		// 조건 마구 추가해보리기

		// 1. 공중에서 움직이는 힘 감소
		// Ground 가 false인 경우, move로 들어오는 force가 감소
		
		// 2. 기본적인 속도 제한 적용 (Clamp)
		// Move로 적용되는 가속도에 한계를 두기
		// Jump, Dash 또한 속도 제한해주기..?




		// force clamp
		// force로 들어간 값은 항상 일정함
		// force가 velocity에 DT곱으로 더해짐
		// 지속적인 force가 friction보다 크기 때문에, 점점 빨라지는 원리임



		// velocity clamp




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

	void Rigidbody::applyGravity()
	{
		Vector2 gravity(0.0f, -9.80f);
		Collider2D* footCollider = GetOwner()->GetFootCollider();

		if (footCollider->IsGround())
		{
			// 현재 속도의 y성분 제거하기
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mTempVelocity -= gravity * dot;
		}
		else
		{
			// 임시 velocity에 중력 적용
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
		// 현재속도가 멈추지 않은 상태인 경우
		if (0 < mTempVelocity.LengthSquared())
		{
			// 마찰력(상수) 계산하기
			Vector2 frictionUnit = -mTempVelocity;
			frictionUnit.Normalize();
			Vector2 friction = frictionUnit * mFriction * mMass * (float)Time::DeltaTime();

			// 추가 입력이 없고, 땅에 닿아있는 경우에만 마찰력 마구 증가함
			if (0 >= mForceDir.LengthSquared())
			{
				Collider2D* footCollider = GetOwner()->GetFootCollider();

				if (footCollider->IsGround())
					friction *= 12.0f;
			}

			// 마찰력이 현재속도보다 큰경우
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
		
		// clamp 해주기
		float velocityMagnitude = mTempVelocity.Length();
		mTempVelocity.Normalize();
		Vector2 calcVelocity;
				
		// ApplyForce 사용 -> ClampForce 적용
		if (0 < mForceDir.LengthSquared())
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -mClampForce / 2.0f, mClampForce / 2.0f);
		}
		// ApplyVelocity 사용 -> ClampVelocity 적용
		else if (0 < mVelocityDir.LengthSquared())
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -mClampVelocity / 2.0f, mClampVelocity / 2.0f);
		}
		// 둘다 아니면 그냥 상수 씀
		else
		{
			calcVelocity = mTempVelocity * std::clamp(velocityMagnitude, -10.0f, 10.0f);
		}

		// 그냥 y축 클램프
		// x축 클램프 따로 두고
		// Dash clamp도 따로 두는게 나을듯함



		// Clamp 적용된 속도를 적용함
		mTotalVelocity = calcVelocity;
	}

	void Rigidbody::applyLocation()
	{
		// 오너 Position 가져오기
		Vector3 retPos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Vector2 calcPos(retPos.x, retPos.y);

		// 내 위치에 속도 더해주기
		calcPos += mTotalVelocity * (float)Time::DeltaTime();

		// 변경된 위치를 갱신해주기
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
		// 지우려는 방향의 단위벡터 구하기
		Vector2 unit = dir;
		unit.Normalize();

		// 단위벡터에 속력을 내적해서 지워야 하는 속력을 구함
		float dot = unit.Dot(velocity);

		// 현재 속도에서 특정 방향의 벡터를 지워줌 
		mVelocity -= unit * dot;
	}
}
