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

	structs::sArmour GameDataManager::mActiveArmour = {};
	structs::sArmour GameDataManager::mSubArmour = {};


	void GameDataManager::Initialize()
	{
		InitializePlayerStat();
	}

	void GameDataManager::InitializePlayerStat()
	{
		mPlayerStat.MaxHP = 30;
		mPlayerStat.CurHP = mPlayerStat.MaxHP;
		mPlayerStat.MoveSpeed = 2.0f;
		mPlayerStat.JumpForce = 4.50f;
		mPlayerStat.DashForce = 6.0f;
		
		mDashCount.MaxDashCount = 2;
		mDashCount.CurDashCount = mDashCount.MaxDashCount;
		mDashCount.DashAccumulateTime = 0.0f;
		mDashCount.DashRegenTime = 1.750f;

		mJumpCount.JumpAccumulateTime = 0.0f;
		mJumpCount.JumpLimitTime = 0.10f;
		mJumpCount.JumpForceRatio = 0.0f;
		mJumpCount.BufferedJump = false;
		mJumpCount.ExtraJump = true;

		mInventoryData.Armour1.Weapon = enums::eWeaponType::LongSword;
		mActiveArmour = mInventoryData.Armour1;
		mSubArmour = mInventoryData.Armour2;
	}

	void GameDataManager::GetDamage(float value)
	{
		mPlayerStat.CurHP -= value;
		if (0 >= mPlayerStat.CurHP)
			mPlayerStat.CurHP = 0;

		SceneManager::GetLifebarScript()->BindConstantBuffer();
	}

	void GameDataManager::GetHeal(float value)
	{
		mPlayerStat.CurHP += value;
		if (mPlayerStat.MaxHP <= mPlayerStat.CurHP)
			mPlayerStat.CurHP = mPlayerStat.MaxHP;
		SceneManager::GetLifebarScript()->BindConstantBuffer();
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

	void GameDataManager::CallInventory()
	{
		SceneManager::GetInventoryScript()->CallInventory();
	}

	void GameDataManager::ChangeArmour()
	{
		// swap armour
		structs::sArmour temp = mActiveArmour;
		mActiveArmour = mSubArmour;
		mSubArmour = temp;

		// change UI
		SceneManager::GetInventoryScript()->ChangeArmour();
	}

	enums::eWeaponType GameDataManager::GetWeaponType()
	{
		return mActiveArmour.Weapon;
	}
}