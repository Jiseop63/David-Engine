#pragma once
#include "daPortalScript.h"

namespace da
{
	class DungeonPortalScript : public PortalScript
	{
	public:
		DungeonPortalScript();
		virtual ~DungeonPortalScript();

		virtual void Initialize() override;
		virtual void OnCollisionEnter(Collider2D* other) override;



	};

}