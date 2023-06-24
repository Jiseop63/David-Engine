#pragma once
#include "daComponent.h"

namespace da
{
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
		void SetProjectionType(eProjectionType type) { mProjectionType = type; }
		bool CreateViewMatrix();
		bool CreateProjectionMatrix();

	private:
		static math::Matrix mView;
		static math::Matrix mProjection;

		eProjectionType mProjectionType;
		float mAspectRatio;
		float mNear;
		float mFar;
		float mSize;
	};
}