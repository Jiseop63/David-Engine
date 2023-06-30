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
		static math::Matrix GetViewMatrix() { return View; }
		static math::Matrix GetProjectionMatrix() { return Projection; }
	
	public:
		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		
	public:
		void TurnLayerMask(enums::eLayerType layer, bool enable = true);
		void EnableLayerMask() { mLayerMask.set(); }
		void DisableLayerMask() { mLayerMask.reset(); }

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

	protected:
		Transform* mTransform;

	private:
		// Global matrix
		static math::Matrix View;
		static math::Matrix Projection;

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