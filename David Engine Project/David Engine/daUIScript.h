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
		bool mFocusOn;

	private:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;

		math::Vector2 mScreenPosition;
		math::Vector2 mSize;
	};
}