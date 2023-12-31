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
			std::vector<Event> ActionEvents;
		};

		Animator();
		virtual ~Animator();
		virtual void LateUpdate();

		void Create(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, math::Vector2 leftTop
			, math::Vector2 size
			, UINT columnLength
			, math::Vector2 offset = math::Vector2::Zero
			, float duration = 0.0f
			, float scale = 30.0f);
		Animation* FindAnimation(const std::wstring& name);
		Events* FindEvents(const std::wstring& name);

		void PlayAnimation(const std::wstring& name, bool loop = true);
		void Binds();

		std::function<void()>& StartEvent(const std::wstring& name);
		std::function<void()>& CompleteEvent(const std::wstring& name);
		std::function<void()>& EndEvent(const std::wstring& name);
		std::function<void()>& ActionEvent(const std::wstring& name, UINT index);


	public:
		Animation* GetActiveAnimation() { return mActiveAnimation; }
		
	private:
		std::map<std::wstring, Animation*>	mAnimations;
		std::map<std::wstring, Events*>		mEvents;
		Animation*							mActiveAnimation;
		bool								mLoop;
	};
}
