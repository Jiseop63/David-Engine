#pragma once
#include "daScript.h"
#include "daTexture.h"

namespace da
{
	class ButtonScript : public Script
	{
	public:
		enum eButtonType
		{
			Play,
			Exit,
		};

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
		void SetButtonType(eButtonType buttonType) { mButtonType = buttonType; }
		eButtonType GetButtonType() { return mButtonType; }
	protected:
		void changeTexture(std::shared_ptr<graphics::Texture> texture);
		void focusCheck();


	protected:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;

		math::Vector2 mScreenPosition;
		math::Vector2 mSize;

		eButtonType mButtonType;
		bool mFocusOn;
	};
}
