#pragma once
#include "daComponent.h"

namespace da
{
	class Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

	private:
		static math::Matrix mView;
		static math::Matrix mProjection;
	};
}