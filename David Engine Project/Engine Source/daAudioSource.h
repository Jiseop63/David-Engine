#pragma once
#include "daComponent.h"
#include "daAudioClip.h"

namespace da
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		virtual ~AudioSource();

		virtual void LateUpdate() override;

		void Play();
		void Play(std::shared_ptr<AudioClip> clip, float volume, bool loop = false);
		void Stop();
		void SetLoop(bool loop);

		void SetClip(std::shared_ptr<AudioClip> clip) { mAudioClip = clip; }
		std::shared_ptr<AudioClip> GetClip() { return mAudioClip; }

	private:
		std::shared_ptr<AudioClip> mAudioClip;
	};
}
