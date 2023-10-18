#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class InputIconScript : public Script
	{
	public:
		InputIconScript();
		virtual ~InputIconScript();

		virtual void Initialize() override;

	public:
		void SetPosition(math::Vector3 position);

	private:
		Transform*		mIconTransform;
		MeshRenderer*	mIconRenderer;
	};
}