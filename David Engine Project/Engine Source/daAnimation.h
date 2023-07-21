#pragma once
#include "daResource.h"
#include "daTexture.h"

namespace da
{
	using namespace graphics;
	class Animator;
	class Animation : public Resource
	{
	public:
		struct Sprite
		{
			math::Vector2 LeftTop;
			math::Vector2 Size;
			math::Vector2 Offset;
			math::Vector2 AtlasSize;
			float Duration;

			Sprite()
				: LeftTop(math::Vector2::Zero)
				, Size(math::Vector2::One)
				, Offset(math::Vector2::Zero)
				, AtlasSize(math::Vector2::One)
				, Duration(0.0f)
			{}
		};

		Animation();
		virtual ~Animation();

		virtual HRESULT Load(const std::wstring& path) { return S_FALSE; }

		void Update();
		void LateUpdate();
		void Render();

		void Create(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, math::Vector2 leftTop
			, math::Vector2 size
			, UINT columnLength
			, math::Vector2 offset = math::Vector2::Zero
			, float duration = 0.0f);

		void Binds();
		void Reset();

	public:
		bool IsComplete() { return mComplete; }

	private:
		Animator*							mAnimator;
		std::shared_ptr<graphics::Texture>	mAtlas;
		std::vector<Sprite>					mSprites;
		bool								mComplete;
		int									mIndex;
		float								mDurationTime;
	};
}