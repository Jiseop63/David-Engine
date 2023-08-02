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
        , mRenderer(nullptr)

        , mBodyCollider(nullptr)
        , mFootCollider(nullptr)
        , mRightCollider(nullptr)
        , mLeftCollider(nullptr)

        , mWeaponScript(nullptr)
        
        , mPlayerDir(math::Vector2::Zero)

        , mActiveState(ePlayerState::Idle)
        , mMoveCondition(0)
        , mDead(false)
        
        , mPlayerStat(nullptr)
        , mJumpCount(nullptr)
        , mDashCount(nullptr)
        , mInventoryData(nullptr)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}

#pragma region Default Func
	void PlayerScript::Initialize()
	{
        mTransform = GetOwner()->GetComponent<Transform>();
        mRigidbody = GetOwner()->GetComponent<Rigidbody>();
        mRenderer = GetOwner()->GetComponent<MeshRenderer>();
        InitAnimation();
        InitCollider();
        mPlayerStat = &GameDataManager::GetPlayerStat();
        mJumpCount = &GameDataManager::GetJumpCount();
        mDashCount = &GameDataManager::GetDashCount();
        mInventoryData = &GameDataManager::GetInventory();

        
	}
    void PlayerScript::Update()
    {
        // Time
        timeProcess();

        // Input
        PlayerInput();
        GetMouse();

        // FSM
        PlayerFSM();

    }
#pragma endregion
#pragma region Input Func
    void PlayerScript::PlayerInput()
    {
        DebugInput();
        UIInput();
        if (mDead)
            return;
        todoMove();
        todoJump();
        todoDash();
        todoAttack();        
    }
    void PlayerScript::DebugInput()
    {
        if (Input::GetKeyDown(eKeyCode::R))
        {
            GetHeal();
            if (0 < mPlayerStat->CurHP)
                ChangeState(ePlayerState::Idle);
        }
        if (Input::GetKeyDown(eKeyCode::T))
        {
            GetDamage();
            if (0 >= mPlayerStat->CurHP)
                ChangeState(ePlayerState::Dead);
        }
    }
    void PlayerScript::UIInput()
    {
        if (Input::GetKeyDown(eKeyCode::V))
        {
            GameDataManager::CallInventory();
        }
        if (Input::GetKeyDown(eKeyCode::TILDE))
        {
            GameDataManager::ChangeArmour();
        }
    }
    void PlayerScript::GetMouse()
    {
        if (mDead)
            return;
        
        CalcPlayerDir();
        ReverseTexture();
    }
    void PlayerScript::CalcPlayerDir()
    {
        Vector3 mouseWorldPosition = Input::GetMouseWorldPosition();
        Vector2 mousePosition(mouseWorldPosition.x, mouseWorldPosition.y);
        Vector3 playerPosition = mTransform->GetPosition();
        // Player Dir
        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);
        playerDir.Normalize();
        mPlayerDir = playerDir;
        mWeaponScript->SetWeaponPosition(Vector3(playerPosition.x, playerPosition.y, 0.0f));
        mWeaponScript->SetPlayerDir(mPlayerDir);
    }
    void PlayerScript::ReverseTexture()
    {
        bool value = false;
        if (0 >= mPlayerDir.x)
            value = true;
        mRenderer->SetReverse(value);
        mWeaponScript->SetReverse(value);
    }

#pragma endregion
#pragma region FSM Func
    void PlayerScript::PlayerFSM()
    {
        switch (mActiveState)
        {
        case da::ePlayerState::Idle:
            HandleIdle();
            break;
        case da::ePlayerState::Move:
            HandleMove();
            break;
        case da::ePlayerState::jumpProcess:
            HandleJump();
            break;
        case da::ePlayerState::Dead:
            HandleDead();
            break;
        default:
            break;
        }
    }
    void PlayerScript::ChangeState(ePlayerState state)
    {
        mActiveState = state;
        switch (mActiveState)
        {
        case da::ePlayerState::Idle:
            mAnimator->PlayAnimation(L"playerIdle");
            break;
        case da::ePlayerState::Move:
            mAnimator->PlayAnimation(L"playerMove");
            break;
        case da::ePlayerState::jumpProcess:
            mAnimator->PlayAnimation(L"playerJump");
            break;
        case da::ePlayerState::Dead:
            mAnimator->PlayAnimation(L"playerDead");
            break;
        default:
            break;
        }

    }
    void PlayerScript::HandleIdle()
    {
        // ->Move
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
            ChangeState(ePlayerState::Move);

        if (Input::GetKeyDown(eKeyCode::SPACE))
            ChangeState(ePlayerState::jumpProcess);
        if (Input::GetKeyDown(eKeyCode::RBTN))
            ChangeState(ePlayerState::jumpProcess);
    }
    void PlayerScript::HandleMove()
    {        
        if (0 == mMoveCondition)
        {
            mRigidbody->SetMoving(false);
            mAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }
        else
            mRigidbody->SetMoving(true);

        if (Input::GetKeyDown(eKeyCode::SPACE))
            ChangeState(ePlayerState::jumpProcess);
        if (Input::GetKeyDown(eKeyCode::RBTN))
            ChangeState(ePlayerState::jumpProcess);
    }

    void PlayerScript::HandleJump()
    {
        if (mFootCollider->IsGround())
            ChangeState(ePlayerState::Idle);
    }
    void PlayerScript::HandleDead()
    {
        mDead = true;

        if (0 < mPlayerStat->CurHP)
        {
            mDead = false;
            ChangeState(ePlayerState::Idle);
        }
    }
    
#pragma endregion
#pragma region Common Func
    void PlayerScript::todoMove()
    {
            Vector3 mPos = mTransform->GetPosition();
        // moveAnimation
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
        {
            mMoveCondition++;
        }
        if (Input::GetKeyUp(eKeyCode::D)
            || Input::GetKeyUp(eKeyCode::A))
            mMoveCondition--;

        if (Input::GetKey(eKeyCode::D))
        {
            //mRigidbody->ApplyForce(Vector2::UnitX, mPlayerStat->MoveSpeed);
            mPos.x += mPlayerStat->MoveSpeed * (float)Time::DeltaTime();
            mTransform->SetPosition(mPos);
        }
        if (Input::GetKey(eKeyCode::A))
        {
            //mRigidbody->ApplyForce(-Vector2::UnitX, mPlayerStat->MoveSpeed);
            mPos.x -= mPlayerStat->MoveSpeed * (float)Time::DeltaTime();
            mTransform->SetPosition(mPos);
        }
    }
    void PlayerScript::todoJump()
    {
        // 점프 개수가 유효하다는 뜻임
        if (mFootCollider->IsGround())
        {
            // 버퍼에 추가
            if (Input::GetKeyDown(eKeyCode::SPACE))
            {
                mJumpCount->BufferedJump = true;
            }
            if (Input::GetKeyUp(eKeyCode::SPACE))
            {
                // 버퍼 중단
                mJumpCount->BufferedJump = false;
                // 바로 점프
                jumpProcess();
            }
        }
        else
        {
            // 점프 개수가 유효한지 확인
            if (mJumpCount->ExtraJump)
            {
                if (Input::GetKeyDown(eKeyCode::SPACE))
                {
                    mJumpCount->ExtraJump = false;
                    mJumpCount->JumpForceRatio = 0.80f;
                    // 점프하기
                    jumpProcess();
                }
            }
        }

    }
    void PlayerScript::todoDash()
    {
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            // condition
            if (GameDataManager::UseDash())
            {
                // to do
                mFootCollider->ApplyGround(false);
                mRigidbody->OverrideVelocity(mPlayerDir, mPlayerStat->DashForce);
            }
        }
    }
    void PlayerScript::todoAttack()
    {
        if (Input::GetKey(eKeyCode::LBTN))
            mWeaponScript->DoAttack();
    }
#pragma endregion
#pragma region Debuging Func
    void PlayerScript::GetDamage()
    {
        float value = 5.0f;
        GameDataManager::GetDamage(value);
    }
    void PlayerScript::GetHeal()    
    {
        float value = 5.0f;
        GameDataManager::GetHeal(value);
        
    }
#pragma endregion
#pragma region Auto Func
    void PlayerScript::timeProcess()
    {
        dashRegen();
        jumpRegen();
        bufferedJump();
    }
    void PlayerScript::dashRegen()
    {
        if (mDashCount->MaxDashCount == mDashCount->CurDashCount)
            return;

        mDashCount->DashAccumulateTime += (float)Time::DeltaTime();
        if (mDashCount->DashRegenTime<= mDashCount->DashAccumulateTime)
            GameDataManager::RecoveryDash();
    }

    void PlayerScript::jumpRegen()
    {
        if (mFootCollider->IsGround())
            mJumpCount->ExtraJump = true;
    }
    
    void PlayerScript::bufferedJump()
    {
        // 버퍼에 등록되면
        if (mJumpCount->BufferedJump)
        {
            // 시간을 재다가
            mJumpCount->JumpAccumulateTime += (float)Time::DeltaTime();

            // 조건에 맞으면 점프
            if (mJumpCount->JumpLimitTime <= mJumpCount->JumpAccumulateTime)
            {
                mJumpCount->JumpAccumulateTime = 0.0f;
                mJumpCount->JumpForceRatio = 1.0f;
                jumpProcess();
            }
        }
    }

    // 여기서 점프가 실행됨
    void PlayerScript::jumpProcess()
    {
        // 이펙트 실행
        //mEffectScript->SetEffectPosition(mTransform->GetPosition() + Vector3(0.0f, -0.450f, 0.0f));
        //mEffectScript->PlayEffect(L"Jumping");

        // 최소 높이 설정
        float minForce = 0.650f;
        if (minForce >= mJumpCount->JumpForceRatio)
            mJumpCount->JumpForceRatio = minForce;
        
        mFootCollider->ApplyGround(false);
        mRigidbody->ApplyVelocity(Vector2::UnitY, mPlayerStat->JumpForce * mJumpCount->JumpForceRatio);

        GameDataManager::ClearJumpBuffer();
    }

#pragma endregion
#pragma region Init Func
    void PlayerScript::InitAnimation()
    {
        mAnimator = GetOwner()->GetComponent<Animator>();

        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerJump", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerDead", texture, Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->PlayAnimation(L"playerIdle");
    }
    void PlayerScript::InitCollider()
    {
        mBodyCollider = GetOwner()->GetComponent<Collider2D>();
        mFootCollider = GetOwner()->AddComponent<Collider2D>();
        mRightCollider = GetOwner()->AddComponent<Collider2D>();
        mLeftCollider = GetOwner()->AddComponent<Collider2D>();

        // body
        {
            mBodyCollider->SetName(L"BodyCollider");
            mBodyCollider->SetSize(Vector2(0.30f, 0.40f));
            mBodyCollider->SetCenter(Vector2(0.0f, -0.10f));
        }
        // foot
        {
            GetOwner()->SetFootCollider(mFootCollider);
            mFootCollider->SetName(L"FootCollider");
            mFootCollider->SetSize(Vector2(0.050f, 0.050f));
            mFootCollider->SetCenter(Vector2(0.0f, -0.450f));
            mFootCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
        }
        // right & left
        {
            mRightCollider->SetName(L"RightCollider");
            mRightCollider->SetSize(Vector2(0.050f, 0.050f));
            mRightCollider->SetCenter(Vector2(0.150f, -0.350f));
            mRightCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);

            mLeftCollider->SetName(L"LeftCollider");
            mLeftCollider->SetSize(Vector2(0.050f, 0.050f));
            mLeftCollider->SetCenter(Vector2(-0.150f, -0.350f));
            mLeftCollider->SetColliderDetection(Collider2D::eColliderDetection::Land);
        }
    }
    WeaponScript* PlayerScript::SetWeaponObject(GameObject* object)
    {
        mWeaponScript = object->AddComponent<WeaponScript>();
        return mWeaponScript;
    }
    EffectScript* PlayerScript::SetEffectObject(GameObject* object)
    {
        mEffectScript = object->AddComponent<EffectScript>();
        return mEffectScript;
    }
#pragma endregion
#pragma region Collision Func
    void PlayerScript::OnLandEnter(Collider2D* other)
    {
    }
    void PlayerScript::OnLandStay(Collider2D* other)
    {
        //// Land Pos, Size 가져오기
        //Vector3 landPos = other->GetTotalPosition();
        //Vector3 landSize = other->GetTotalScale();
        //// 내 Pos, Size 가져오기
        //Vector3 myPos = mFootCollider->GetTotalPosition();
        //Vector3 mySize = mFootCollider->GetTotalScale();

        //// A 상자의 최소/최대 위치
        //Vector3 minLand = landPos - landSize * 0.5f;
        //Vector3 maxLand = landPos + landSize * 0.5f;

        //// B 상자의 최소/최대 위치
        //Vector3 minMe = myPos - mySize * 0.5f;
        //Vector3 maxMe = myPos + mySize * 0.5f;

        //// 정황상 플레이어가 바닥에 파고들었음
        //if (minLand.x <= maxMe.x && maxLand.x >= minMe.x &&
        //    minLand.y <= maxMe.y && maxLand.y >= minMe.y &&
        //    minLand.z <= maxMe.z && maxLand.z >= minMe.z)
        //{
        //    float landHeight = maxLand.y - minLand.y;
        //    float myHeight = maxMe.y - minMe.y;
        //    float totalHeight = landHeight / 2.0f + myHeight / 2.0f;

        //    float collisionDepth = myPos.y - landPos.y;
        //    float pushDistance = 0.0f;
        //    if (collisionDepth < totalHeight)
        //    {
        //        pushDistance = totalHeight - collisionDepth;
        //    }
        //    Vector3 newPosition = mTransform->GetPosition();
        //    newPosition.y += pushDistance;
        //    mTransform->SetPosition(newPosition);            
        //}
    }
#pragma endregion
}