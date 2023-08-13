#pragma once
#include "daComponent.h"

namespace da
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();
		
		virtual void LateUpdate() override;

	public:
		void BindConstantBuffer();

	public:
		void SetPosition(math::Vector3 position) { mPosition = position; }
		void SetRotation(math::Vector3 rotation) { mRotation = rotation;}
		void SetScale(math::Vector3 scale) { mScale = scale;}
		void SetOffset(math::Vector3 offset) { mOffset = offset; }
		void SetPosition(float x, float y, float z) { mPosition = math::Vector3(x, y, z); }
		void SetPosition(math::Vector2 vector2) { mPosition.x = vector2.x; mPosition.y = vector2.y; }

		void SetRotation(float x, float y, float z) { mRotation = math::Vector3(x, y, z); }
		void SetScale(float x, float y, float z) { mScale = math::Vector3(x, y, z); }
		void SetOffset(float x, float y, float z) { mOffset = math::Vector3(x, y, z); }

		math::Vector3 GetPosition() { return mPosition; }
		math::Vector3 GetRotation() { return mRotation; }
		math::Vector3 GetScale() { return mScale; }
		math::Vector3 GetOffset() { return mOffset; }
		math::Vector3 GetTotalPosition() { return mTotalPosition; }

		math::Vector3 Forward() { return mForward; }
		math::Vector3 Right() { return mRight; }
		math::Vector3 Up() { return mUp; }


		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		math::Matrix& GetWorldMatrix() { return mWorld; }
		// 내 스크린 좌표 반환하는 함수
	public:
		math::Vector3  GetScreenPosition();
		math::Viewport GetViewport();


	private:
		Transform* mParent;
		math::Matrix mWorld;
		
		math::Vector3 mPosition;
		math::Vector3 mRotation;
		math::Vector3 mScale;
		math::Vector3 mOffset;
		math::Vector3 mTotalPosition;

		math::Vector3 mUp;
		math::Vector3 mForward;
		math::Vector3 mRight;

	private:
		// 안씀
		math::Vector3 mScreenPos;

	};
}