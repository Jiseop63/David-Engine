#pragma once
#include "daComponent.h"
#include "daTransform.h"

namespace da
{
	class Collider2D : public Component
	{
	public:
		enum class eColliderDetection
		{
			Default,
			Land,			// 바닥 충돌 전용
			Inactive,
			End,
		};

		Collider2D();
		virtual ~Collider2D();

		virtual void Initialize() override;
		virtual void LateUpdate() override;

		void ChangingCollisionColor(bool isCollision);
		void ChangingLandColor(bool isCollision);

	public:
		void SetColliderType(enums::eColliderShape type) { mColliderShape = type; }
		enums::eColliderShape GetColliderType() { return mColliderShape; }
		void SetColliderDetection(eColliderDetection type);
		eColliderDetection GetColliderDetection() { return mDetectionType; }

		void SetSize(math::Vector2 size) { mSize = size; }
		math::Vector2 GetSize() { return mSize; }
		
		void SetCenter(math::Vector2 size) { mCenter = size; }
		math::Vector2 GetCenter() { return mCenter; }
		bool IsGround() { return mGrounded; }
		void ApplyGround(bool isGround) { mGrounded = isGround; }

		math::Vector3 GetTotalPosition() { return mTotalPosition; }
		math::Vector3 GetTotalScale() { return mTotalScale; }

	public:
		void OnCollisionEnter(Collider2D* other);
		void OnCollisionStay(Collider2D* other);
		void OnCollisionExit(Collider2D* other);

		void OnLandEnter(Collider2D* other);
		void OnLandStay(Collider2D* other);
		void OnLandExit(Collider2D* other);
		
	public:
		UINT GetColliderID() { return mColliderID; }

	protected:
		Transform* mTransform;

	private:
		static UINT				ColliderNumber;
		UINT					mColliderID;

		// type value
		enums::eColliderShape	mColliderShape;
		eColliderDetection		mDetectionType;
		math::Vector4			mColliderColor;
		// in value
		math::Vector2			mSize;
		math::Vector2			mCenter;

	private:
		// ret Value
		math::Vector3			mTotalPosition;
		math::Vector3			mTotalScale;

	private:
		bool					mGrounded;
	};
}
