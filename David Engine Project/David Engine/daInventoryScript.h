#pragma once
#include "daScript.h"
#include "daGameObject.h"
#include "daTexture.h"

namespace da
{
	class InventoryScript : public Script
	{
	public:
		InventoryScript();
		virtual ~InventoryScript();

		virtual void Update() override;
		
		void Open();
		void Close();

	public:
		void SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);
		void ChangeTexture(std::shared_ptr<graphics::Texture> texture);

	protected:
		bool mInventoryOpen;
		bool mSelectLeft;
		
		// ���� 2�� ���� 2�� ��ű� 4�� ������ 15��
		std::vector<GameObject*> mWeapon;
		std::vector<GameObject*> mShiled;
		std::vector<GameObject*> mAccessory;
		std::vector<GameObject*> mItem;

	private:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;
	};
}
