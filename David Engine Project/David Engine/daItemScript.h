#pragma once
#include "daScript.h"
#include "daTexture.h"

namespace da
{
	class ItemScript : public Script
	{
		enum class eItemType
		{
			Weapon,
			Shiled,
			Accessory,
			Item,
		};
	public:
		ItemScript();
		virtual ~ItemScript();

		virtual void Update() override;

	public:
		void SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void SetScreenPosision();

	protected:
		void changeTexture(std::shared_ptr<graphics::Texture> texture);
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
