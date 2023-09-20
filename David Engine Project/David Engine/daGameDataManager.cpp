#include "daGameDataManager.h"

#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"

namespace da
{
	using namespace math;
	structs::sInventory GameDataManager::mInventoryData = {};

	structs::sArmour* GameDataManager::mActiveArmour = nullptr;
	structs::sArmour* GameDataManager::mSubArmour = nullptr;

	da::math::Vector2 GameDataManager::mCameraMovableRange = Vector2::Zero;
	da::math::Vector2 GameDataManager::mCameraMovePosition = Vector2::Zero;
	
	bool GameDataManager::mDebuging = false;

	void GameDataManager::Initialize()
	{
		InitializeInventory();
	}

	void GameDataManager::InitializeInventory()
	{
		mActiveArmour = new structs::sArmour();
		mActiveArmour->Weapon.WeaponName = enums::eWeaponName::LongSword;
		mSubArmour = new structs::sArmour();

		mInventoryData.Armour1 = *mActiveArmour;
		mInventoryData.Armour2 = *mSubArmour;
	}


	void GameDataManager::SetCameraMovaPosition(da::math::Vector2 vector2, bool shake)
	{		
		math::Vector2 calcPos;
		if (vector2.x <= mCameraMovableRange.x
			&& vector2.x >= -mCameraMovableRange.x)
			calcPos.x = vector2.x;
		else if (vector2.x <= -mCameraMovableRange.x)
			calcPos.x = -mCameraMovableRange.x;
		else if (vector2.x >= mCameraMovableRange.x)
			calcPos.x = mCameraMovableRange.x;

		if (vector2.y <= mCameraMovableRange.y
			&& vector2.y >= -mCameraMovableRange.y)
			calcPos.y = vector2.y;
		else if (vector2.y <= -mCameraMovableRange.y)
			calcPos.y = -mCameraMovableRange.y;
		else if (vector2.y >= mCameraMovableRange.y)
			calcPos.y = mCameraMovableRange.y;
		
		if (shake)
			mCameraMovePosition = vector2;
		else
			mCameraMovePosition = calcPos;
	}

	void GameDataManager::CallInventory()
	{
		SceneManager::GetInventoryScript()->CallInventory();
	}

	void GameDataManager::ChangeArmour()
	{
		//// swap armour
		//structs::sArmour temp = *mActiveArmour;
		//mActiveArmour = mSubArmour;
		//mSubArmour = &temp;

		//// change UI
		//SceneManager::GetInventoryScript()->ChangeArmour();
		//// 패널 이미지 변경해주기
		//// SceneManager::GetHUDObject()->Get
		//
		//
		//// control weaponScript
		//SceneManager::GetPlayerScript()->GetWeaponScript()->EquipWeapon();
	}
	structs::sArmour* GameDataManager::GetActiveArmour()
	{
		return mActiveArmour;
	}
}