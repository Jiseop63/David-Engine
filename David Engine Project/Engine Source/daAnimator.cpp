#include "daAnimator.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	Animator::Animator()
		: Component(enums::eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation(nullptr)
		, mLoop(false)
	{
	}
	Animator::~Animator()
	{
		for (auto iter : mAnimations)
		{
			delete iter.second;
			iter.second = nullptr;
		}
		for (auto iter : mEvents)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
	void Animator::LateUpdate()
	{
		if (!mUsing)
			return;
		if (!mActiveAnimation)
			return;
		// 애니메이션에 있는 이벤트 가져오기
		Events* events = FindEvents(mActiveAnimation->GetKey());

		// 애니메이션 재생이 완료됨
		if (mActiveAnimation->IsComplete())
		{
			if (events)
				events->CompleteEvent();
			if (mLoop)
				mActiveAnimation->Reset();
		}

		UINT frameIndex = mActiveAnimation->LateUpdate();
		if (events)
		{
			// Complete가 아니고, 프레임 중간에 ActionEvent가 있다면 그것을 실행
			if (frameIndex != -1 && events->ActionEvents[frameIndex].mEvent)
			{
				events->ActionEvents[frameIndex].mEvent();
			}
		}
	}
	void Animator::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas, math::Vector2 leftTop
		, math::Vector2 size, UINT columnLength, math::Vector2 offset
		, float duration, float scale)
	{
		Animation* animation = FindAnimation(name);
		if (nullptr != animation)
			return;

		animation = new Animation();
		animation->SetKey(name);
		animation->Create(name, atlas, leftTop, size, columnLength, offset, duration, scale);
		mAnimations.insert( std::make_pair(name, animation) );

		Events* events = FindEvents(name);
		if (events)
			return;

		events = new Events();
		events->ActionEvents.resize(columnLength);
		mEvents.insert( std::make_pair(name, events) );

	}
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}
	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		std::map<std::wstring, Events*>::iterator iter
			= mEvents.find(name);

		if (iter == mEvents.end())
			return nullptr;

		return iter->second;
	}
	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* prevAnimation = mActiveAnimation;
		Events* events;
		// EndEvent 조건
		if (prevAnimation)
		{
			events = FindEvents(prevAnimation->GetKey());
			if (events)
				events->EndEvent();
		}

		// 다음 애니메이션 실행
		Animation* animation = FindAnimation(name);
		if (animation)
		{
			mActiveAnimation = animation;

			events = FindEvents(mActiveAnimation->GetKey());
			if (events)
				events->StartEvent();
		}

		mLoop = loop;
		mActiveAnimation->Reset();
	}
	void Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;
		mActiveAnimation->Binds();
	}
	std::function<void()>& Animator::StartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->StartEvent.mEvent;
	}
	std::function<void()>& Animator::CompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->CompleteEvent.mEvent;
	}
	std::function<void()>& Animator::EndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->EndEvent.mEvent;
	}
	std::function<void()>& Animator::ActionEvent(const std::wstring& name, UINT index)
	{
		Events* events = FindEvents(name);

		return events->ActionEvents[index].mEvent;
	}
}