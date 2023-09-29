#pragma once
#include "daPortalScript.h"

namespace da
{
	class TownPortalScript : public PortalScript
	{
	public:
		TownPortalScript();
		virtual ~TownPortalScript();

		virtual void Initialize() override;
		virtual void OnCollisionEnter(Collider2D* other) override;

	public:
		void HidePlayer();

	};

}