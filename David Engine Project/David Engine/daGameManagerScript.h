#pragma once
#include "daScript.h"

namespace da
{
	class GameManagerScript : public Script
	{
	public:
		GameManagerScript();
		virtual ~GameManagerScript();

		virtual void Update() override;
	private:

	};
}