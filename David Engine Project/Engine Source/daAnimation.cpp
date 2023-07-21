#include "daAnimation.h"
#include "daTime.h"
#include "daAnimator.h"
#include "daRenderer.h"

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
	{
	}
	Animation::~Animation()
	{
	}
	void Animation::LateUpdate()
	{
		if (mComplete)
			return;
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
		}
	}
	void Animation::Create(const std::wstring& name
		, std::shared_ptr<graphics::Texture> atlas
		, math::Vector2 leftTop, math::Vector2 size
		, UINT columnLength, math::Vector2 offset, float duration)
	{
		SetKey(name);
		mAtlas = atlas;

		float scale = 50.0f;

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
			sprite.AtlasSize = math::Vector2(scale / width, scale /height);
			sprite.Duration = duration;

			mSprites.push_back(sprite);
		}
	}
	void Animation::Binds()
	{
		// texture bind
		mAtlas->BindShader(graphics::eShaderStage::PS, 12);

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