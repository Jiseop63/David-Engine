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
		, mReverse(false)
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
		if (nullptr == mActiveAnimation)
			return;

		if (mActiveAnimation->IsComplete()
			&& true == mLoop)
		{
			Events* events = FindEvents(mActiveAnimation->GetKey());
			if (events)
				events->CompleteEvent();

			mActiveAnimation->Reset();
		}

		mActiveAnimation->LateUpdate();
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
		animation->Create(name, atlas, leftTop, size, columnLength, offset, duration);
		mAnimations.insert( std::make_pair(name, animation) );

		Events* events = FindEvents(name);
		if (events)
			return;

		events = new Events();
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
		if (prevAnimation)
		{
			events = FindEvents(prevAnimation->GetKey());
			if (events)
				events->EndEvent();
		}		

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

		renderer::ReverseCB cbData = {};
		cbData.Reverse = mReverse;

		graphics::ConstantBuffer* reverseCB = renderer::constantBuffer[(UINT)graphics::eCBType::Reverse];
		reverseCB->SetData(&cbData);
		reverseCB->Bind(graphics::eShaderStage::VS);
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
}