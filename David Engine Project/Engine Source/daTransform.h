#pragma once
#include "daComponent.h"

namespace da
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
	public:
		void BindConstantBuffer();

	public:
		void SetPosition(math::Vector3 position) { mPosition = position; }
		void SetRotation(math::Vector3 rotation) { mRotation = rotation;}
		void SetScale(math::Vector3 scale) { mScale = scale;}
		void SetPosition(float x, float y, float z) { mPosition = math::Vector3(x, y, z); }
		void SetRotation(float x, float y, float z) { mRotation = math::Vector3(x, y, z); }
		void SetScale(float x, float y, float z) { mScale = math::Vector3(x, y, z); }

		math::Vector3 GetPosition(math::Vector3 position) { return mPosition; }
		math::Vector3 GetRotation(math::Vector3 rotation) { return mRotation; }
		math::Vector3 GetScale(math::Vector3 scale) { return mScale; }

	public:
		math::Vector3 mPosition;
		math::Vector3 mRotation;
		math::Vector3 mScale;
	};
}