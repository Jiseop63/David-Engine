#pragma once
#include "daComponent.h"
#include "daTransform.h"

namespace da
{
	class Collider2D : public Component
	{
	public:
		Collider2D();
		~Collider2D();

		virtual void Initialize() override;
		virtual void LateUpdate() override;

		void SetColliderType(enums::eColliderType type) { mColliderType = type; }
		enums::eColliderType GetColliderType() { return mColliderType; }

		void SetSize(math::Vector2 size) { mSize = size; }
		math::Vector2 GetSize() { return mSize; }
		
		void SetCenter(math::Vector2 size) { mCenter = size; }
		math::Vector2 GetCenter() { return mCenter; }

	public:
		void OnCollisionEnter(Collider2D* other);
		void OnCollisionStay(Collider2D* other);
		void OnCollisionExit(Collider2D* other);
		
	public:
		UINT GetColliderID() { return mColliderID; }

	private:
		static UINT ColliderNumber;
		UINT mColliderID;
		enums::eColliderType mColliderType;
		Transform* mTransform;

		math::Vector3 mPosition;
		math::Vector2 mSize;
		math::Vector2 mCenter;

	};
}
