#include "daAudioListener.h"
#include "daTransform.h"
#include "daGameObject.h"
#include "daFmod.h"

namespace da
{
	AudioListener::AudioListener()
		: Component(enums::eComponentType::AudioListener)
	{
	}
	void AudioListener::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();
		math::Vector3 foward = tr->Forward();
		math::Vector3 up = tr->Up();

		math::Vector3 vel = { 0.0f, 0.0f, 0.0f };
		Fmod::Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}
}
