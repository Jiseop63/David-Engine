#pragma once
#include "daComponent.h"
#include "daAnimation.h"

namespace da
{
	class Animator : public Component
	{
	public:
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}
			void operator()()
			{
				if (mEvent)
					mEvent();
			}
			std::function<void()> mEvent;
		};
		struct Events
		{
			Event StartEvent;
			Event CompleteEvent;
			Event EndEvent;
		};

		Animator();
		virtual ~Animator();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		void Create(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, math::Vector2 leftTop
			, math::Vector2 size
			, UINT columnLength
			, math::Vector2 offset = math::Vector2::Zero
			, float duration = 0.0f);
		Animation* FindAnimation(const std::wstring& name);
		void PlayAnimation(const std::wstring& name, bool loop = true);
		void Binds();

	private:
		std::map<std::wstring, Animation*>	mAnimations;
		std::map<std::wstring, Events*>		mEvents;
		Animation*							mActiveAnimation;
		bool								mLoop;
	};
}
