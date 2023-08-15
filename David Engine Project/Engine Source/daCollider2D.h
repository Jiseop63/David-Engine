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
		};
		enum class eWallCollisionState
		{
			None,
			Left,
			Right,
			Top,
			LT,
			RT,
		};

		Collider2D();
		virtual ~Collider2D();

		virtual void Initialize() override;
		virtual void LateUpdate() override;

		void ChangeCollisionColor(bool isCollision);
		void ChangeLandColor(bool isCollision);
		void ChangeColliderColor(bool isCollision);

	public:	
		void SetColliderType(enums::eColliderShape type) { mColliderShape = type; }
		enums::eColliderShape GetColliderType() { return mColliderShape; }
		
		void SetDetectionType(eDetectionType type);
		eDetectionType GetDetectionType() { return mDetectionType; }

		void SetSize(math::Vector2 size) { mSize = size; }
		math::Vector2 GetSize() { return mSize; }
		
		void SetCenter(math::Vector2 size) { mCenter = size; }
		math::Vector2 GetCenter() { return mCenter; }

		bool IsGround() { return mGrounded; }
		void ApplyGround(bool isGround) { mGrounded = isGround; }

		eWallCollisionState IsWallCollision() { return mWallCollision; }

		math::Vector3 GetTotalPosition() { return mTotalPosition; }
		math::Vector3 GetTotalScale() { return mTotalScale; }

		void ImFoot(bool isFoot = true) { mFoot = isFoot; }
		void ImBody(bool isBody = true) { mBody = isBody; }

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
		// condition value
		bool					mFoot;
		bool					mBody;
		bool					mGrounded;			// Foot Collider
		eWallCollisionState		mWallCollision;		// Body Collider
	};
}
