#pragma once
#include "daScript.h"
#include "daTexture.h"
#include "daMeshRenderer.h"

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

		// ������ ���ִ� ��
	public:
		void SetSlotTextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void SetScreenPosision();

		// Manager�� ���� ȣ��
	public:
		void ChangeItemTexture(std::shared_ptr<graphics::Texture> texture);
		bool IsFocus() { return mFocusOn; }

	protected:
		void ChangeSlotTexture(std::shared_ptr<graphics::Texture> texture);
		void focusCheck();

	protected:
		MeshRenderer* mSlotRenderer;
		MeshRenderer* mItemRenderer;

		// ���콺 �Է�
	protected:
		bool mFocusOn;
		math::Vector2 mScreenPosition;
		math::Vector2 mSize;

		// ��� �ؽ��� ��
	private:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;

		
	};
}
