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
			Land,			// �ٴ� �浹 ����
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
		math::Vector3 GetColliderPosition() { return mPosition; }


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

		enums::eColliderShape	mColliderShape;
		eColliderDetection		mDetectionType;

		math::Vector3			mPosition;
		math::Vector2			mSize;
		math::Vector2			mCenter;
		math::Vector4			mColliderColor;

	private:
	};
}
