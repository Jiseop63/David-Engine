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

		// 힘 초기화
		mForce = Vector2::Zero;
		mAcceleration = Vector2::Zero;
		mForceMagnitude = 0.0f;
	}

	void Rigidbody::CalculateVelocity()
	{
		// 가속도 구하기 (A = F / M)
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
			// y 속도 제거하기
			gravity.Normalize();
			// 땅에 파고들었으면 위로 올려주기
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			// 아래로 힘 적용
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


			// 추가 입력이 없으면 마찰력 n배
			if (0 >= mForce.LengthSquared())
				friction *= 12.0f;

			// 기본 마찰력
			if (mVelocity.LengthSquared() < friction.LengthSquared())
				mVelocity = Vector2::Zero;
			else
				mVelocity += friction;
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
