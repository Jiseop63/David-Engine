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

		void SetType(enums::eColliderType type) { mColliderType = type; }
		void SetSize(math::Vector2 size) { mSize = size; }
		void SetCenter(math::Vector2 size) { mCenter = size; }

	private:
		enums::eColliderType mColliderType;
		Transform* mTransform;

		math::Vector3 mPosition;
		math::Vector2 mSize;
		math::Vector2 mCenter;

	};
}
