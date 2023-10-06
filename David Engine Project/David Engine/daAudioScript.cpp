#include "daAudioScript.h"
#include "daGameObject.h"

namespace da
{
	AudioScript::AudioScript()
	{
	}
	AudioScript::~AudioScript()
	{
	}
	void AudioScript::LateUpdate()
	{
		if (mFallowTarget)
			GetOwner()->GetTransform()->SetPosition(mFallowTarget->GetTransform()->GetPosition());
		else
			GetOwner()->GetTransform()->SetPosition(math::Vector3(0.0f, 0.0f, 0.0f));
	}
}
