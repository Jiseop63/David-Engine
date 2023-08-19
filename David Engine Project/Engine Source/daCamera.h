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
		static math::Matrix GetGPUViewMatrix() { return View; }
		static math::Matrix GetGPUProjectionMatrix() { return Projection; }
		static void SetGPUViewMatrix(math::Matrix matrix) { View = matrix; }
		static void SetGPUProjectionMatrix(math::Matrix matrix) { Projection = matrix; }
	public:
		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void LateUpdate() override;
		virtual void Render() override;		
#pragma region layerMask Func
	public:
		void TurnLayerMask(enums::eLayerType layer, bool enable = true);
		void EnableLayerMask() { mLayerMask.set(); }
		void DisableLayerMask() { mLayerMask.reset(); }
#pragma endregion
#pragma region create Matrix
	private:
		bool createViewMatrix();
		bool createProjectionMatrix();
		void registerCameraInRanderer();
#pragma endregion
#pragma region Object sort
		void sortGameObjects();
		void depthSortTransparencyGameobjects();
		void layerSortAnddivideRenderTypeObjects(const std::vector<GameObject*> objects);
		void renderOpaque();
		void renderCutout();
		void renderTransparent();

		void enableDepthStencilState();
		void disableDepthStencilState();
#pragma endregion
	public:
		math::Matrix& GetViewMatrix() { return mView; }
		math::Matrix& GetProjectionMatrix() { return mProjection; }

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

		math::Matrix mView;
		math::Matrix mProjection;

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