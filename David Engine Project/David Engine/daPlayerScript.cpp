#include "daPlayerScript.h"

#include "daGameObject.h"

#include "daInput.h"
#include "daTime.h"
#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daResources.h"

#include "daCreatureScript.h"
#include "daWeaponScript.h"
#include "daEffectPlayerScript.h"

namespace da
{
    PlayerScript::PlayerScript()
        : mTransform(nullptr)
        , mRigidbody(nullptr)
        , mAnimator(nullptr)
        , mRenderer(nullptr)
        , mLight(nullptr)
        , mBodyCollider(nullptr)
        , mFootCollider(nullptr)

        , mWeaponScript(nullptr)
        , mEffects{}

        , mPlayerDir(math::Vector2::Zero)

        , mActiveState(ePlayerState::Idle)
        , mpreviousState(ePlayerState::Idle)
        , mMoveCondition(0)
        , mDead(false)
        , mDustAccumulateTime(0.0f)
                
        , mPlayerStat(nullptr)
        , mJumpCount(nullptr)
        , mDashCount(nullptr)

        , mDashRunning(false)
        , mHoldingDashTime(0.0f)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}

#pragma region Default Func
	void PlayerScript::Initialize()
	{
        mTransform = GetOwner()->GetComponent<Transform>();
        mRenderer = GetOwner()->GetComponent<MeshRenderer>();
        mLight = GetOwner()->GetComponent<Light>();

        mRigidbody = GetOwner()->AddComponent<Rigidbody>();
        InitAnimation();
        InitCollider();

        mPlayerStat = &GameDataManager::GetPlayerStat();
        mJumpCount = &GameDataManager::GetJumpCount();
        mDashCount = &GameDataManager::GetDashCount();
        ChangeWeapon();
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
#pragma region Common Func
    void PlayerScript::PlayerInput()
    {
        DebugInput();
        UIInput();
        if (mDead)
            return;
        CameraMove();
        InputMove();
        inputJump();
        inputDash();
        InputAttack();        
    }
    void PlayerScript::DebugInput()
    {
        if (Input::GetKeyDown(eKeyCode::R))
        {
            GetHeal();
            if (0 < mPlayerStat->CurHP)
            {
                mDead = false;
                ChangeState(ePlayerState::Idle);
                mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
            }
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

        bool value = IsLeft();
        mRenderer->SetReverse(value);

        mWeaponScript->SetWeaponTransform(playerPosition, mPlayerDir);
    }
    void PlayerScript::timeProcess()
    {
        dashRegen();
        jumpRegen();
        bufferedJump();
        walkDust();

        if (mDashRunning)
        {
            mHoldingDashTime += (float)Time::DeltaTime();
            if (0.30f <= mHoldingDashTime)
            {
                mHoldingDashTime = 0.0f;
                mDashRunning = false;
                mRigidbody->GravityAble(true);
                mRigidbody->OverrideVelocity(math::Vector2::UnitY, 0.010f);
            }
        }
    }       

    EffectPlayerScript* PlayerScript::callEffect()
    {
        for (size_t effect = 0; effect < mEffects.size(); effect++)
        {
            if (GameObject::eObjectState::Inactive ==
                mEffects[effect]->GetOwner()->GetObjectState())
                return mEffects[effect];
        }
        return nullptr;
    }
    void PlayerScript::activeEffect(EffectPlayerScript* effect, const std::wstring name)
    {
        if (!effect)
            return;
        effect->SetEffectPosition(mTransform->GetPosition() + Vector3(0.0f, -0.20f, 0.0f));
        effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
        effect->PlayEffect(name);
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
#pragma region FSM Func
    void PlayerScript::ChangeState(ePlayerState state)
    {
        mpreviousState = mActiveState;
        mActiveState = state;
        switch (mActiveState)
        {
        case da::ePlayerState::Idle:
            mAnimator->PlayAnimation(L"playerIdle");
            break;
        case da::ePlayerState::Move:
            mAnimator->PlayAnimation(L"playerMove");
            break;
        case da::ePlayerState::Jump:
            mAnimator->PlayAnimation(L"playerJump");
            break;
        case da::ePlayerState::Dead:
            mAnimator->PlayAnimation(L"playerDead");
            break;
        default:
            break;
        }
    }
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
        case da::ePlayerState::Jump:
            HandleJump();
            break;
        case da::ePlayerState::Dead:
            HandleDead();
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
    }
    void PlayerScript::HandleJump()
    {
        if (mFootCollider->IsGround())
            ChangeState(mpreviousState);
    }
    void PlayerScript::HandleDead()
    {
        if (!mDead)
        {
            EffectPlayerScript* playerEffect = callEffect();
            playerEffect->SetReverse(IsLeft());
            activeEffect(playerEffect, L"Dying");
            mWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
        }
            
        mDead = true;
    }
#pragma endregion
#pragma region Weapon Logic
    void PlayerScript::InputAttack()
    {
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            // 비활성이라면 활성화 시켜줌
            if (GameObject::eObjectState::Active != mWeaponScript->GetOwner()->GetObjectState())
                mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
            mWeaponScript->DoAttack();
        }
    }
    void PlayerScript::ChangeWeapon()
    {
        // 일단 임시로 무기 세팅
        //mWeaponScript->ChangeWeapon(enums::eWeaponType::LongSword);
    }
#pragma endregion
#pragma region Move Logic
    void PlayerScript::CameraMove()
    {
        Vector3 playerPos = mTransform->GetPosition();
        
        GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
    }
    void PlayerScript::InputMove()
    {        
        Vector3 mPos = mTransform->GetPosition();
        float moveMagnitude = mPlayerStat->MoveSpeed * (float)Time::DeltaTime();

        // moveAnimation
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
            mMoveCondition++;
        if (Input::GetKeyUp(eKeyCode::D)
            || Input::GetKeyUp(eKeyCode::A))
            mMoveCondition--;

        Collider2D::eWallCollisionState wallCollisionState = mBodyCollider->IsWallCollision();

        if (mDashRunning)
            return;

        if (Input::GetKey(eKeyCode::D))
        {        
            if (Collider2D::eWallCollisionState::Right == wallCollisionState)
                return;
            else
                mPos.x += moveMagnitude;
        }
        if (Input::GetKey(eKeyCode::A))
        {
            if (Collider2D::eWallCollisionState::Left == wallCollisionState)
                return;
            else
                mPos.x -= moveMagnitude;
        }
        
       

        mTransform->SetPosition(mPos);
    }
    void PlayerScript::walkDust()
    {
        if (ePlayerState::Move != mActiveState)
            return;

        mDustAccumulateTime += (float)Time::DeltaTime();
        if (0.30f <= mDustAccumulateTime)
        {            
            EffectPlayerScript* playerEffect = callEffect();
            playerEffect->SetReverse(IsLeft());
            activeEffect(playerEffect, L"Walking");
            mDustAccumulateTime = 0.0f;
        }
    }
#pragma endregion
#pragma region Jump & Dash Logic
        void PlayerScript::jumpRegen()
        {
            if (!mJumpCount->ExtraJump)
                if (mFootCollider->IsGround())
                    mJumpCount->ExtraJump = true;
        }
        void PlayerScript::dashRegen()
        {
            if (mDashCount->MaxDashCount == mDashCount->CurDashCount)
                return;

            mDashCount->DashAccumulateTime += (float)Time::DeltaTime();
            if (mDashCount->DashRegenTime <= mDashCount->DashAccumulateTime)
                GameDataManager::RecoveryDash();
        }
        void PlayerScript::inputDash()
        {
            if (Input::GetKeyDown(eKeyCode::RBTN))
            {
                // condition
                if (GameDataManager::UseDash())
                {
                    if (ePlayerState::Jump != mActiveState)
                        ChangeState(ePlayerState::Jump);
                    todoDash();
                }
            }
        }
        void PlayerScript::todoDash()
        {
            // 이동시키기
            mRigidbody->OverrideVelocity(mPlayerDir, mPlayerStat->DashForce);
            mHoldingDashTime = 0.0f;
            mDashRunning = true;
            mRigidbody->GravityAble(false);

            //// 투사체 정보 갱신
            //mWeaponScript->ModifyProjectile(
            //    math::Vector2(0.80f, 1.20f), 0.0f, 0.30f, enums::eProjectileType::Body);
            //// 투사체 활성화
            //mWeaponScript->ActiveProjectile();
        }
        void PlayerScript::inputJump()
        {
            if (mFootCollider->IsGround())
            {
                // 버퍼에 추가
                if (Input::GetKeyDown(eKeyCode::SPACE))
                    mJumpCount->BufferedJump = true;

                if (Input::GetKeyUp(eKeyCode::SPACE))
                {
                    // 버퍼 중단
                    mJumpCount->BufferedJump = false;
                    // 바로 점프
                    todoJump();
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
                        todoJump();
                    }
                }
            }

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
                    todoJump();
                }
            }
        }
        void PlayerScript::todoJump()
        {
            if (ePlayerState::Jump != mActiveState)
                ChangeState(ePlayerState::Jump);
            EffectPlayerScript* playerEffect = callEffect();
            playerEffect->SetReverse(IsLeft());
            activeEffect(playerEffect, L"Jumping");

            // 최소 높이 설정
            float minForceRatio = 0.750f;
            if (minForceRatio >= mJumpCount->JumpForceRatio)
                mJumpCount->JumpForceRatio = minForceRatio;

            mFootCollider->ApplyGround(false);

            if (mJumpCount->ExtraJump)
                mRigidbody->ApplyVelocity(Vector2::UnitY, mPlayerStat->JumpForce * mJumpCount->JumpForceRatio);
            else
                mRigidbody->OverrideVelocity(Vector2::UnitY, mPlayerStat->JumpForce * mJumpCount->JumpForceRatio);
            GameDataManager::ClearJumpBuffer();
        }
#pragma endregion

#pragma region Initialize Player
    void PlayerScript::InitAnimation()
    {
        mAnimator = GetOwner()->AddComponent<Animator>();

        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerJump", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->Create(L"playerDead", texture, Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mAnimator->PlayAnimation(L"playerIdle");
    }
    void PlayerScript::InitCollider()
    {
        mBodyCollider = GetOwner()->AddComponent<Collider2D>();
        mFootCollider = GetOwner()->AddComponent<Collider2D>();

        // body
        {
            GetOwner()->SetBodyCollider(mBodyCollider);
            mBodyCollider->SetName(L"BodyCollider");
            mBodyCollider->ImBody();
            mBodyCollider->SetSize(Vector2(0.30f, 0.40f));
            mBodyCollider->SetCenter(Vector2(0.0f, -0.10f));
            mBodyCollider->SetDetectionType(Collider2D::eDetectionType::Default);
        }
        // foot
        {
            GetOwner()->SetFootCollider(mFootCollider);
            mFootCollider->SetName(L"FootCollider");
            mFootCollider->ImFoot();
            mFootCollider->SetSize(Vector2(0.050f, 0.050f));
            mFootCollider->SetCenter(Vector2(0.0f, -0.450f));
            mFootCollider->SetDetectionType(Collider2D::eDetectionType::Default);
        }
    }
#pragma endregion
#pragma region Collision Func
    void PlayerScript::OnCollisionEnter(Collider2D* other)
    {
    }
    void PlayerScript::OnGroundStay(Collider2D* other)
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
#pragma region public Func
    WeaponScript* PlayerScript::SetWeaponObject(GameObject* object)
    {
        mWeaponScript = object->AddComponent<WeaponScript>();
        mWeaponScript->SetPlayerScript(this);
        return mWeaponScript;
    }
    EffectPlayerScript* PlayerScript::AddEffectObject(GameObject* object)
    {
        EffectPlayerScript* effect = object->AddComponent<EffectPlayerScript>();
        mEffects.push_back(effect);
        return effect;
    }
#pragma endregion
}