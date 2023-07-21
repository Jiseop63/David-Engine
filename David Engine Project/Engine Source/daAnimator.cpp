#include "daAnimator.h"

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
	void Animator::Initialize()
	{
	}
	void Animator::Update()
	{
		if (nullptr == mActiveAnimation)
			return;

		if (mActiveAnimation->IsComplete()
			&& true == mLoop)
		{
			mActiveAnimation->Reset();
		}

		mActiveAnimation->LateUpdate();
	}
	void Animator::LateUpdate()
	{
	}
	void Animator::Render()
	{
	}
	void Animator::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas, math::Vector2 leftTop
		, math::Vector2 size, UINT columnLength, math::Vector2 offset, float duration)
	{
		Animation* animation = FindAnimation(name);
		if (nullptr != animation)
			return;

		animation = new Animation();
		animation->SetKey(name);
		animation->Create(name, atlas, leftTop, size, columnLength, offset, duration);
		mAnimations.insert( std::make_pair(name, animation) );
	}
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}
	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* animation = FindAnimation(name);
		if (nullptr != animation)
			mActiveAnimation = animation;
		mLoop = loop;
		mActiveAnimation->Reset();
	}
	void Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;
		mActiveAnimation->Binds();
	}
}