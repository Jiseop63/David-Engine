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
        mRigidbody->ApplyForce(dir * mPlayerStat->MoveSpeed);
    }

    void PlayerScript::Dash()
    {
        // condition
        if (0 >= mDashCount->CurCount)
            return;
        mDashCount->CurCount--;

        // to do

    }
    void PlayerScript::Jump()
    {
    }
    void PlayerScript::GetDamage()
    {
        float value = 5.0f;

        mPlayerStat->CurHP -= value;
        if (0 >= mPlayerStat->MaxHP)
            mPlayerStat->CurHP = 0;
    }

    void PlayerScript::GetHeal()
    {
        float value = 5.0f;

        mPlayerStat->CurHP += value;
        if (mPlayerStat->MaxHP <= mPlayerStat->CurHP)
            mPlayerStat->CurHP = mPlayerStat->MaxHP;
    }
    void PlayerScript::regenDashCount()
    {
        if (mDashCount->MaxCount == mDashCount->CurCount)
            return;

        mDashCountTime += (float)Time::DeltaTime();

        if (mRegenCountTime <= mDashCountTime)
        {
            mDashCount->CurCount++;
            mDashCountTime = 0.0f;
        }
    }

    void PlayerScript::OnCollisionEnter(Collider2D* other)
    {
    }
    void PlayerScript::OnCollisionStay(Collider2D* other)
    {
    }
    void PlayerScript::OnCollisionExit(Collider2D* other)
    {
    }
}