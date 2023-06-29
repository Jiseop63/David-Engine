#pragma once
#include "daComponent.h"

namespace da
{
	class Transform;
	class Camera : public Component
	{
	public:
		enum class eProjectionType
		{
			Perspective,
			Orthographic,
		};
	public:
		static math::Matrix GetViewMatrix() { return mView; }
		static math::Matrix GetProjectionMatrix() { return mProjection; }
	
	public:
		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		
	public:
		void TurnLayerMask(enums::eLayerType layer, bool enable = true);
		void EnableLayerMask() {}
		void DisableLayerMask() {}

	private:
		bool createViewMatrix();
		bool createProjectionMatrix();
		void registerCameraInRanderer();


		void sortGameObjects();
		void renderOpaque();
		void renderCutout();
		void renderTransparent();

	public:
		float GetSize() { return mSize; }
		void SetSize(float size) { mSize = size; };
		eProjectionType GetProjectionType() { return mProjectionType; }
		void SetProjectionType(eProjectionType type) { mProjectionType = type; }

	private:
		Transform* mTransform;

	private:
		static math::Matrix mView;
		static math::Matrix mProjection;

		eProjectionType mProjectionType;
		float mAspectRatio;
		float mNear;
		float mFar;
		float mSize;

	private:
		std::bitset<(UINT)enums::eLayerType::End> mLayerMask;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
	};
}