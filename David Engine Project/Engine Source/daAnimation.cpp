#include "daAnimation.h"
#include "daTime.h"
#include "daAnimator.h"
#include "daRenderer.h"
#include "daGameObject.h"

namespace da
{
	Animation::Animation()
		: Resource(enums::eResourceType::Animation)
		, mAnimator(nullptr)
		, mAtlas(nullptr)
		, mSprites{}
		, mComplete(false)
		, mIndex(-1)
		, mDurationTime(0.0f)
		, mScale(0.0f)
	{
	}
	Animation::~Animation()
	{
	}
	UINT Animation::LateUpdate()
	{
		if (mComplete)
			return -1;
		mDurationTime += (float)Time::DeltaTime();

		if (mSprites[mIndex].Duration <= mDurationTime)
		{
			mIndex++;
			mDurationTime = 0.0f;

			if (mIndex >= mSprites.size())
			{
				mIndex = (int)mSprites.size() - 1;
				mComplete = true;
			}
			return mIndex;
		}
		return -1;
	}
	void Animation::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas
		, math::Vector2 leftTop, math::Vector2 size
		, UINT columnLength, math::Vector2 offset, float duration, float scale)
	{
		SetKey(name);
		mAtlas = atlas;
		mScale = scale;

		float width = (float)atlas->GetWidth();
		float height = (float)atlas->GetHeight();

		for (size_t index = 0; index < columnLength; index++)
		{
			Sprite sprite = {};
			sprite.LeftTop.x = leftTop.x + (index * size.x) / width;
			sprite.LeftTop.y = leftTop.y / height;
			sprite.Size.x = size.x / width;
			sprite.Size.y = size.y / height;
			sprite.Offset = offset;
			sprite.AtlasSize = math::Vector2(mScale / width, mScale /height);
			sprite.Duration = duration;

			mSprites.push_back(sprite);
		}
	}
	void Animation::Binds()
	{
		// texture bind
		mAtlas->BindShaderResource(graphics::eShaderStage::PS, 12);

		// animation CB
		renderer::AnimationCB cbData = {};
		cbData.SpriteLeftTop = mSprites[mIndex].LeftTop;
		cbData.SpriteSize = mSprites[mIndex].Size;
		cbData.SpriteOffset = mSprites[mIndex].Offset;
		cbData.AtlasSize = mSprites[mIndex].AtlasSize;

		ConstantBuffer* animationCB = renderer::constantBuffer[(UINT)graphics::eCBType::Animation];
		animationCB->SetData(&cbData);
		animationCB->Bind(eShaderStage::VS);
		animationCB->Bind(eShaderStage::PS);
	}
	void Animation::Reset()
	{
		mDurationTime = 0.0f;
		mComplete = false;
		mIndex = 0;
	}
}