#pragma once
#include "daComponent.h"
#include "daTransform.h"

namespace da
{
	class Collider2D : public Component
	{
	public:
		enum class eCollider
		{

		};

		Collider2D();
		virtual ~Collider2D();

		virtual void Initialize() override;
		virtual void LateUpdate() override;

		void BindConstantBuffer();


	public:
		void SetColliderType(enums::eColliderShape type) { mColliderShape = type; }
		enums::eColliderShape GetColliderType() { return mColliderShape; }
		void SetColliderColor(enums::eColliderColor type) { mColorType = type; }
		enums::eColliderColor GetColliderColor() { return mColorType; }

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

	protected:
		Transform* mTransform;

	private:
		static UINT				ColliderNumber;
		UINT					mColliderID;

		enums::eColliderShape	mColliderShape;
		enums::eColliderColor	mColorType;

		math::Vector3			mPosition;
		math::Vector2			mSize;
		math::Vector2			mCenter;

	private:
	};
}
