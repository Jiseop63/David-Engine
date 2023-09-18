#pragma once
#include "daComponent.h"
#include "daTransform.h"

namespace da
{
	class Collider2D : public Component
	{
	public:

		enum class eDetectionType
		{
			Default,
			Env,
			Sensor,
			Inactive,
		};
		enum class eWallCollisionState
		{
			None,
			Left,
			Right,
			Top,
		};

		Collider2D();
		virtual ~Collider2D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ChangeCollisionColor(bool isCollision);
		void ChangeLandColor(bool isCollision);
		void ChangeColliderColor(bool isCollision);

	public:
		void SetColliderShapeType(enums::eColliderShape type) { mColliderShape = type; }
		enums::eColliderShape GetColliderType() { return mColliderShape; }
		
		void SetDetectionType(eDetectionType type);
		eDetectionType GetDetectionType() { return mDetectionType; }

		void SetSize(math::Vector2 size) { mSize = size; }
		math::Vector2 GetSize() { return mSize; }
		
		void SetCenter(math::Vector2 size) { mCenter = size; }
		math::Vector2 GetCenter() { return mCenter; }

		math::Vector3 GetTotalPosition() { return mTotalPosition; }
		math::Vector3 GetTotalScale() { return mTotalScale; }

		bool IsCollision() { return mIsCollision; }
		bool IsGround() { return  0 < mGroundBuffer ? true : false; }

		// 이건 안쓸 예정임
		void ApplyGround(bool isGround) { mGrounded = isGround; }

		int GetGroundBuffer() { return mGroundBuffer; }
		void ClearGroundBuffer(int value = 0) { mGroundBuffer = value; }

		bool IsPlatformCollision() { return mPlatformCollision; }
		eWallCollisionState IsWallCollision() { return mWallCollision; }

		void ImFoot(bool isFoot = true) { mFoot = isFoot; }
		bool IsFoot() { return mFoot; }
		void ImBody(bool isBody = true) { mBody = isBody; }
		bool IsBody() { return mBody; }

		float GetEnvRotate() { return mEnvRotate; }

	private:
		void groundCheck(Collider2D* other, bool isEnter);
		void wallCollisionCheck(Collider2D* other, bool isEnter);

	public:
		void OnCollisionEnter(Collider2D* other);
		void OnCollisionStay(Collider2D* other);
		void OnCollisionExit(Collider2D* other);

	public:
		UINT GetColliderID() { return mColliderID; }

	protected:
		Transform* mTransform;

	private:
		static UINT				ColliderNumber;
		UINT					mColliderID;
		// ret Value
		math::Vector3			mTotalPosition;
		math::Vector3			mTotalScale;

		// visual value
		enums::eColliderShape	mColliderShape;
		eDetectionType			mDetectionType;
		math::Vector4			mColliderColor;
		math::Vector2			mSize;
		math::Vector2			mCenter;




	private:
		// Creature 유형의 충돌조건 함수
		bool					mFoot;
		bool					mBody;
		bool					mIsCollision;
		bool					mGrounded;			// Foot Collider
		int						mGroundBuffer;
		bool					mPlatformCollision;
		eWallCollisionState		mWallCollision;		// Body Collider




		// 경사로 이동간 충돌에서 예외처리를 위한 바닥충돌 버퍼
	private:
		float					mEnvRotate;
		float					mEnvRotateBuffer;
		math::Vector3			mEnvPositionBuffer;
	};
}
