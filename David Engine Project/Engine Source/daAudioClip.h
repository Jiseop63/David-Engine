#pragma once
#include "daResource.h"
#include "daFmod.h"

namespace da
{

	class AudioClip : public Resource
	{
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual HRESULT Load(const std::wstring& path) override;

		void Play();
		void Stop();
		void Set3DAttributes(const math::Vector3 pos, const math::Vector3 vel);
		void SetLoop(bool loop) { mbLoop = loop; }
		void SetVolume(float volume);

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

