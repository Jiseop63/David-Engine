#include "daGameDataManager.h"

#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"

namespace da
{
	using namespace math;
	structs::sCreatureStat GameDataManager::mPlayerStat = {};
	structs::sJumpData GameDataManager::mJumpData = {};
	structs::sDashData GameDataManager::mDashData = {};
	structs::sInventory GameDataManager::mInventoryData = {};

	structs::sArmour* GameDataManager::mActiveArmour = nullptr;
	structs::sArmour* GameDataManager::mSubArmour = nullptr;

	da::math::Vector2 GameDataManager::mCameraMovableRange = Vector2::Zero;
	da::math::Vector2 GameDataManager::mCameraMovePosition = Vector2::Zero;
	
	bool GameDataManager::mDebuging = false;

	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 3.50f;
		
		mDashData.DashForce = 10.50f;
		mDashData.MaxDashCount = 2;
		mDashData.CurDashCount = mDashData.MaxDashCount;
		mDashData.DashAccumulateTime = 0.0f;
		mDashData.DashRegenTime = 2.250f;

		mJumpData.JumpForce = 8.50f;
		mJumpData.JumpAccumulateTime = 0.0f;
		mJumpData.JumpLimitTime = 0.1250f;
		mJumpData.JumpForceRatio = 0.0f;
		mJumpData.BufferedJump = false;
		mJumpData.ExtraJump = true;

		mActiveArmour = new structs::sArmour();
		mActiveArmour->Weapon.WeaponName = enums::eWeaponName::LongSword;
		mSubArmour = new structs::sArmour();

		mInventoryData.Armour1 = *mActiveArmour;
		mInventoryData.Armour2 = *mSubArmour;
	}

	void GameDataManager::GetDamage(float value)
	{
		mPlayerStat.CurHP -= value;
		if (0 >= mPlayerStat.CurHP)
			mPlayerStat.CurHP = 0;

		SceneManager::GetLifebarScript()->SetValue(mPlayerStat.MaxHP, mPlayerStat.CurHP);
	}

	void GameDataManager::GetHeal(float value)
	{
		mPlayerStat.CurHP += value;
		if (mPlayerStat.MaxHP <= mPlayerStat.CurHP)
			mPlayerStat.CurHP = mPlayerStat.MaxHP;

		SceneManager::GetLifebarScript()->SetValue(mPlayerStat.MaxHP, mPlayerStat.CurHP);
	}

	bool GameDataManager::UseDash()
	{		
		if (0 < mDashData.CurDashCount)
		{
			mDashData.CurDashCount -= 1;
			SceneManager::GetDashCountScript()->BindConstantBuffer();
			return true;
		}
		SceneManager::GetDashCountScript()->BindConstantBuffer(); 
		return false;
	}

	void GameDataManager::RecoveryDash()
	{
		if (mDashData.MaxDashCount > mDashData.CurDashCount)
		{
			mDashData.DashAccumulateTime = 0.0f;
			mDashData.CurDashCount += 1;
			SceneManager::GetDashCountScript()->BindConstantBuffer();
		}
	}

	void GameDataManager::ClearJumpBuffer()
	{
		mJumpData.BufferedJump = false;
		mJumpData.JumpForceRatio = 0.0f;
		mJumpData.JumpAccumulateTime = 0.0f;
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
		// swap armour
		structs::sArmour temp = *mActiveArmour;
		mActiveArmour = mSubArmour;
		mSubArmour = &temp;

		// change UI
		SceneManager::GetInventoryScript()->ChangeArmour();
		// 패널 이미지 변경해주기
		// SceneManager::GetHUDObject()->Get
		
		
		// control weaponScript
		SceneManager::GetPlayerScript()->GetWeaponScript()->EquipWeapon();
	}
	structs::sArmour* GameDataManager::GetActiveArmour()
	{
		return mActiveArmour;
	}
}