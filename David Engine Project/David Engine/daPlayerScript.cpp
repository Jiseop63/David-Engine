#include "daPlayerScript.h"

#include "daInput.h"
#include "daTime.h"
#include "daGameDataManager.h"

#include "daGameObject.h"

namespace da
{
    PlayerScript::PlayerScript()
        : mRigidbody(nullptr)
        , mMoveSpeed(6.0f)
        , mRegenCountTime(1.750f)
        , mDashCountTime(0.0f)
        , mPlayerStat(nullptr)
        , mDashCount(nullptr)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
        mRigidbody = GetOwner()->GetComponent<Rigidbody>();
        mPlayerStat = &GameDataManager::GetPlayerStat();
        mDashCount = &GameDataManager::GetDashCount();
	}
    void PlayerScript::Update()
    {
        GetInput();
        regenDashCount();
    }
    void PlayerScript::GetInput()
    {
        if (Input::GetKey(eKeyCode::D))
        {
            MoveFunc(Vector2::UnitX);
        }
        if (Input::GetKey(eKeyCode::A))
        {
            MoveFunc(-Vector2::UnitX);
        }
        if (Input::GetKeyDown(eKeyCode::R))
        {
            GetHeal();
        }
        if (Input::GetKeyDown(eKeyCode::T))
        {
            GetDamage();
        }
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            Dash();
        }
    }
    void PlayerScript::MoveFunc(Vector2 dir)
    {
        //mRigidbody->ApplyV2Force(dir * mMoveSpeed);
        
        mRigidbody->ApplyV2Force(dir * mPlayerStat->MoveSpeed);
    }
    void PlayerScript::Dash()
    {
        //// condition
        //structs::sDashCount& myStat = GameDataManager::GetDashCount();
        //if (0 >= myStat.CurCount)
        //    return;
        //myStat.CurCount--;

        //// to do

        if (0 >= mDashCount->CurCount)
            return;
        mDashCount->CurCount--;
    }
    void PlayerScript::Jump()
    {
    }
    void PlayerScript::GetDamage()
    {
        float value = 5.0f;
        // structs::sCreatureStat& myStat = GameDataManager::GetPlayerStat();
        // myStat.CurHP -= value;
        // if (0 >= myStat.CurHP)
        //     myStat.CurHP = 0;

        mPlayerStat->CurHP -= value;
        if (0 >= mPlayerStat->MaxHP)
            mPlayerStat->CurHP = 0;
    }

    void PlayerScript::GetHeal()
    {
        float value = 5.0f;
        /*structs::sCreatureStat& myStat = GameDataManager::GetPlayerStat();
        myStat.CurHP += value;
        
        if (myStat.MaxHP <= myStat.CurHP)
            myStat.CurHP = myStat.MaxHP;*/

        mPlayerStat->CurHP += value;
        if (mPlayerStat->MaxHP <= mPlayerStat->CurHP)
            mPlayerStat->CurHP = mPlayerStat->MaxHP;
    }
    void PlayerScript::regenDashCount()
    {
        //// 최대치인경우 스킵
        //structs::sDashCount& myStat = GameDataManager::GetDashCount();
        //if (myStat.MaxCount == myStat.CurCount)
        //    return;

        //mDashCountTime += Time::DeltaTime();

        //if (mRegenCountTime <= mDashCountTime)
        //{
        //    myStat.CurCount++;
        //    mDashCountTime = 0.0f;
        //}

        if (mDashCount->MaxCount == mDashCount->CurCount)
            return;

        mDashCountTime += (float)Time::DeltaTime();

        if (mRegenCountTime <= mDashCountTime)
        {
            mDashCount->CurCount++;
            mDashCountTime = 0.0f;
        }
    }
}