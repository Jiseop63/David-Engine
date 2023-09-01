#pragma once
#include "daComponent.h"

namespace da
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		virtual ~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
	};
}