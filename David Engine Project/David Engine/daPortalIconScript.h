#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class PortalIconScript : public Script
	{
	public:
		PortalIconScript();
		virtual ~PortalIconScript();

		virtual void Initialize() override;

	public:
		void SetPosition(math::Vector3 position);
		void ApplyIcon(bool value = true);

	private:
		Transform*		mIconTransform;
		MeshRenderer*	mIconRenderer;
	};
}