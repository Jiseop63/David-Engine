#include "daPlayerScript.h"

#include "daGameObject.h"

#include "daInput.h"
#include "daTime.h"
#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daResources.h"

namespace da
{
    PlayerScript::PlayerScript()
        : mTransform(nullptr)
        , mRigidbody(nullptr)
        , mAnimator(nullptr)
        , mWeaponTransform(nullptr)
        , mWeaponRenderer(nullptr)
        , mMoveSpeed(6.0f)
        , mRegenCountTime(1.750f)
        , mDashCountTime(0.0f)
        , mPlayerStat(nullptr)
        , mDashCount(nullptr)
        , mInventory(nullptr)
        , mMoveCondition(0)
        , mReverse(false)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
        mTransform = GetOwner()->GetComponent<Transform>();
        mRigidbody = GetOwner()->GetComponent<Rigidbody>();
        mAnimator = GetOwner()->GetComponent<Animator>();
        InitAnimation();

        mPlayerStat = &GameDataManager::GetPlayerStat();
        mDashCount = &GameDataManager::GetDashCount();
        mInventory = &GameDataManager::GetInventory();
	}
    void PlayerScript::Update()
    {
        GetInput();
        GetMouse();
        WeaponMove();
        regenDashCount();
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
            {
                mMoveCondition--;
                if (0 == mMoveCondition)
                    mAnimator->PlayAnimation(L"playerIdle");
            }

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
                if (0 < mPlayerStat->CurHP)
                    mAnimator->PlayAnimation(L"playerIdle");
            }
            if (Input::GetKeyDown(eKeyCode::T))
            {
                GetDamage();
                if (0 >= mPlayerStat->CurHP)
                    mAnimator->PlayAnimation(L"playerDead");
            }
        }
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            Dash();
        }
    }

    void PlayerScript::GetMouse()
    {
        Vector3 mouseWorldPosition = Input::GetMouseWorldPosition();
        Vector2 mousePosition(mouseWorldPosition.x, mouseWorldPosition.y);
        Vector2 mouseDir = mousePosition;
        mouseDir.Normalize();
        Vector3 playerPosition = mTransform->GetPosition();

        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);
        
        playerDir.Normalize();
        if (0 <= playerDir.x)
            mAnimator->SetReverse(false);
        else
            mAnimator->SetReverse(true);

        Vector3 weaponPosition(playerPosition.x, playerPosition.y, 0.0f);
        float angle = atan2(mouseDir.y, mouseDir.x) - atan2(mTransform->Up().y, mTransform->Up().x);
        
        mWeaponTransform->SetPosition(weaponPosition);
        mWeaponTransform->SetRotation(Vector3(0.0f,0.0f, angle - 3.925f));

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
        Vector3 mousePosition = Input::GetMouseWorldPosition();

    }
    void PlayerScript::Jump()
    {
    }
    void PlayerScript::WeaponMove()
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
    void PlayerScript::SetWeaponObject(GameObject* object)
    {
        mWeaponObject = object;
        mWeaponTransform = mWeaponObject->GetComponent<Transform>();
        mWeaponRenderer = mWeaponObject->GetComponent<MeshRenderer>();
        
        std::shared_ptr<Material> weaponMaterial = mWeaponRenderer->GetMaterial();
        weaponMaterial->SetTexture(Resources::Find<Texture>(L"GreatSwordTexture"));
        // 9 22
        //mWeaponTransform->SetScale(Vector3(0.180f, 0.440f, 1.0f));
        mWeaponTransform->SetScale(Vector3(2.0f, 2.0f, 1.0f));
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

    void PlayerScript::InitAnimation()
    {
        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerJump", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerDead", texture, Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->PlayAnimation(L"playerIdle");
    }

}