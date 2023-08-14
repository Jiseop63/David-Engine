#include "daGameDataManager.h"

#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"

namespace da
{
	using namespace math;
	structs::sPlayerStat GameDataManager::mPlayerStat = {};
	structs::sJumpCount GameDataManager::mJumpCount = {};
	structs::sDashCount GameDataManager::mDashCount = {};
	structs::sInventory GameDataManager::mInventoryData = {};

	structs::sArmour* GameDataManager::mActiveArmour = nullptr;
	structs::sArmour* GameDataManager::mSubArmour = nullptr;

	da::math::Vector2 GameDataManager::mCameraMovableRange = Vector2::Zero;
	da::math::Vector2 GameDataManager::mCameraMovePosition = Vector2::Zero;

	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 3.50f;
		mPlayerStat.JumpForce = 5.50f;
		mPlayerStat.DashForce = 7.50f;
		
		mDashCount.MaxDashCount = 2;
		mDashCount.CurDashCount = mDashCount.MaxDashCount;
		mDashCount.DashAccumulateTime = 0.0f;
		mDashCount.DashRegenTime = 2.250f;

		mJumpCount.JumpAccumulateTime = 0.0f;
		mJumpCount.JumpLimitTime = 0.1750f;
		mJumpCount.JumpForceRatio = 0.0f;
		mJumpCount.BufferedJump = false;
		mJumpCount.ExtraJump = true;

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
		if (0 < mDashCount.CurDashCount)
		{
			mDashCount.CurDashCount -= 1;
			SceneManager::GetDashCountScript()->BindConstantBuffer();
			return true;
		}
		SceneManager::GetDashCountScript()->BindConstantBuffer(); 
		return false;
	}

	void GameDataManager::RecoveryDash()
	{
		if (mDashCount.MaxDashCount > mDashCount.CurDashCount)
		{
			mDashCount.DashAccumulateTime = 0.0f;
			mDashCount.CurDashCount += 1;
			SceneManager::GetDashCountScript()->BindConstantBuffer();
		}
	}

	void GameDataManager::ClearJumpBuffer()
	{
		mJumpCount.BufferedJump = false;
		mJumpCount.JumpForceRatio = 0.0f;
		mJumpCount.JumpAccumulateTime = 0.0f;
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
		SceneManager::GetPlayerScript()->GetWeaponScript()->ChangeWeapon();
	}
	structs::sArmour* GameDataManager::GetActiveArmour()
	{
		return mActiveArmour;
	}
}