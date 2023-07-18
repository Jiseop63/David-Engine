#pragma once
#include "daScript.h"
#include "daTransform.h"

namespace da
{
	class SubCameraScript : public Script
	{
	public:
		SubCameraScript();
		virtual ~SubCameraScript();

		virtual void Update();

		void SetMainCameraTransfrom(Transform* mainTransform) { mMainCameraTransform = mainTransform; }
	private:
		Transform* mMainCameraTransform;
	};
}