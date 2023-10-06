#include "daAudioSource.h"
#include "daAudioClip.h"
#include "daTransform.h"
#include "daGameObject.h"

namespace da
{
	AudioSource::AudioSource()
		: Component(enums::eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{
	}
	AudioSource::~AudioSource()
	{
	}
	void AudioSource::LateUpdate()
	{
		/*Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();
		math::Vector3 foward = tr->Forward();

		mAudioClip->Set3DAttributes(pos, foward);*/
	}
	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Play(std::shared_ptr<AudioClip> clip, float volume, bool loop)
	{
		if (!clip)
			int a = 0;
		clip->SetVolume(volume);
		clip->SetLoop(loop);
		clip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}
}
