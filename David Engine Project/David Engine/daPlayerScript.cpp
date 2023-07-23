#include "daPlayerScript.h"

#include "daInput.h"
#include "daTime.h"
#include "daGameDataManager.h"
#include "daResources.h"
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
        mAnimator = GetOwner()->GetComponent<Animator>();
        mPlayerStat = &GameDataManager::GetPlayerStat();
        mDashCount = &GameDataManager::GetDashCount();
        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerMove", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerJump", texture, Vector2(0.0f, 128.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->PlayAnimation(L"playerIdle");
	}
    void PlayerScript::Update()
    {
        GetInput();
        PlayAnimation();
        regenDashCount();

       /* if (Vector2::Zero == mRigidbody->GetVelocity())
            mAnimator->PlayAnimation(L"playerIdle");*/
    }
    void PlayerScript::LateUpdate()
    {
    }
    void PlayerScript::GetInput()
    {
        // move
        {
            if (Input::GetKeyDown(eKeyCode::D)
                || Input::GetKeyDown(eKeyCode::A))
            {
                mAnimator->PlayAnimation(L"playerMove");
                mMoveCondition++;
            }
                
            if (Input::GetKeyUp(eKeyCode::D)
                || Input::GetKeyUp(eKeyCode::A))
                mMoveCondition--;

            if (Input::GetKey(eKeyCode::D))
            {
                MoveFunc(Vector2::UnitX);
            }
            if (Input::GetKey(eKeyCode::A))
            {
                MoveFunc(-Vector2::UnitX);
            }
        }
        // hp debug
        {
            if (Input::GetKeyDown(eKeyCode::R))
            {
                GetHeal();
            }
            if (Input::GetKeyDown(eKeyCode::T))
            {
                GetDamage();
            }
        }
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            Dash();
        }
    }

    void PlayerScript::PlayAnimation()
    {
        if (0 == mMoveCondition)
            mAnimator->PlayAnimation(L"playerIdle");
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

    void PlayerScript::Complete()
    {
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