#pragma once
#include "daScript.h"
#include "daTexture.h"

namespace da
{
	class ButtonScript : public Script
	{
	public:
		ButtonScript();
		virtual ~ButtonScript();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void SetScreenPosision();

	protected:
		void changeTexture(std::shared_ptr<graphics::Texture> texture);
		void isCollisionMouse();


	protected:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;

		math::Vector2 mScreenPosition;
		math::Vector2 mSize;

		bool mIsChanged;
	};
}
