#pragma once
#include "daScript.h"
#include "daGameObject.h"
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

	protected:
		// ���� 2��
		std::vector<GameObject*> mWeapon;
		// ���� 2��
		std::vector<GameObject*> mShiled;
		// ��ű� 4��
		std::vector<GameObject*> mAccessory;
		// ������ 15��
		std::vector<GameObject*> mItem;
	};
}
