#pragma once
#include "daScript.h"
#include "daTexture.h"

namespace da
{
	class UIScript : public Script
	{
	public:
		UIScript();
		virtual ~UIScript();

		virtual void Update();

	public:
		void SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void SetScreenPosision();
		void ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture);

	protected:
		void focusCheck();

	protected:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;

		bool mFocusOn;
		math::Vector2 mScreenPosition;
		math::Vector2 mSize;
	};
}