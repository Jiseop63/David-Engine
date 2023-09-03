#pragma once
#include "daComponent.h"

namespace da
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		virtual ~AudioListener();

		virtual void LateUpdate() override;

	private:
	};
}