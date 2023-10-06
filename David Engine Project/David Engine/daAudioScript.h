#pragma once
#include "daScript.h"
#include "daAudioClip.h"

namespace da
{
	class AudioScript : public Script
	{
	public:
		AudioScript();
		virtual ~AudioScript();

		virtual void LateUpdate();

		void SetFallowTarget(GameObject* target) { mFallowTarget = target; }

	private:
		GameObject* mFallowTarget;
	};
}