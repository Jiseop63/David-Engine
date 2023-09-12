#include "daPlayerScript.h"

#include "daGameObject.h"

#include "daInput.h"
#include "daTime.h"
#include "daGameDataManager.h"

#include "daConstantBuffer.h"
#include "daRenderer.h"
#include "daResources.h"

#include "daMonsterScript.h"
#include "daPlayerCombatScript.h"
#include "daPlayerEffectScript.h"

namespace da
{
    PlayerScript::PlayerScript()
        : mLight(nullptr)
        , mWeaponScript(nullptr)

        , mActiveState(ePlayerState::Idle)
        , mpreviousState(ePlayerState::Idle)
        , mMoveCondition(0)

        , mDustAccumulateTime(0.0f)

        , mJumpData{}
        , mDashData{}

        , mJumping(false)
        , mDashRunning(false)
        , mHoldingDashTime(0.0f)

        , mPassPlatform(false)
        , mBodyCollision(false)
        , mFootCollision(false)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}

#pragma region Default Func
	void PlayerScript::Initialize()
	{
        CreatureScript::Initialize();
        mLight = GetOwner()->GetComponent<Light>();

        initializeAnimation();
        initializeCollider();
        initializeData();
        
	}
    void PlayerScript::Update()
    {
        updateDashRegen();
        updateJumpRegen();
        updateBufferedJump();
        updatePassPlatformCheck();
        updateDashValidTime();

        updatePlayerConditionCheck();
        updatePlayerInput();
        updatePlayerFSM();
    }
    void PlayerScript::LateUpdate()
    {
        updatePlayerDir();
        CreatureScript::visualUpdate();

        updateWalkEffect();
    }
#pragma endregion
#pragma region Common Func
    void PlayerScript::updatePlayerConditionCheck()
    {        
        if (0 >= mCreatureStat.CurHP)
            ChangeState(ePlayerState::Dead);
    }
    void PlayerScript::updatePlayerInput()
    {
        inputDebug();
        inputUI();
        if (mIsDead)
            return;
        inputCameraMove();
        inputMove();
        inputDash();
        inputAttack();        
    }
    void PlayerScript::inputDebug()
    {
        if (Input::GetKeyDown(eKeyCode::R))
            GetHeal(); 
        if (Input::GetKeyDown(eKeyCode::E))
            downDashCount();
        if (Input::GetKeyDown(eKeyCode::T))
            GetDamage();
        if (Input::GetKeyDown(eKeyCode::Y))
        {
            GameDataManager::DebugMode();
            bool isDebug = GameDataManager::IsDebuging();
            int a = 0;
        }
    }
    void PlayerScript::inputUI()
    {
        if (Input::GetKeyDown(eKeyCode::V))
            GameDataManager::CallInventory();
        if (Input::GetKeyDown(eKeyCode::TILDE))
            GameDataManager::ChangeArmour();
    }
    void PlayerScript::updatePlayerDir()
    {
        if (mIsDead)
            return;
        Vector3 mouseWorldPosition = Input::GetMouseWorldPosition();
        Vector2 mousePosition(mouseWorldPosition.x, mouseWorldPosition.y);
        Vector3 playerPosition = mCreatureTransform->GetPosition();
        // Player Dir
        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);
        playerDir.Normalize();
        mCreatureDir = playerDir;
    }
    void PlayerScript::todoActiveEffect(EffectScript* effect, const std::wstring name)
    {
        if (!effect)
            return;
        effect->SetEffectPosition(mCreatureTransform->GetPosition() + Vector3(0.0f, -0.20f, 0.0f));
        effect->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
        effect->PlayEffect(name);
    }
#pragma endregion
#pragma region Debuging Func
    void PlayerScript::AddEffectObject(GameObject* effectObject)
    {
        PlayerEffectScript* enemyEffect = effectObject->AddComponent<PlayerEffectScript>();
        mEffects.push_back(enemyEffect);
    }
    void PlayerScript::GetDamage()
    {
        float value = 5.0f;
        mCreatureStat.CurHP -= value;
        if (0 >= mCreatureStat.CurHP)
            mCreatureStat.CurHP = 0;
        //GameDataManager::GetDamage(value);
    }
    void PlayerScript::GetHeal()    
    {
        float value = 5.0f;
        mCreatureStat.CurHP += value;
        if (mCreatureStat.MaxHP <= mCreatureStat.CurHP)
            mCreatureStat.CurHP = mCreatureStat.MaxHP;
        //GameDataManager::GetHeal(value);        
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
            mCreatureAnimator->PlayAnimation(L"playerIdle");
            break;
        case da::ePlayerState::Move:
            mCreatureAnimator->PlayAnimation(L"playerMove");
            break;
        case da::ePlayerState::Jump:
            mCreatureAnimator->PlayAnimation(L"playerJump");
            break;
        case da::ePlayerState::Dead:
            mCreatureAnimator->PlayAnimation(L"playerDead");
            break;
        default:
            break;
        }
    }
    void PlayerScript::updatePlayerFSM()
    {
        switch (mActiveState)
        {
        case da::ePlayerState::Idle:
            handleIdle();
            break;
        case da::ePlayerState::Move:
            handleMove();
            break;
        case da::ePlayerState::Jump:
            handleJump();
            break;
        case da::ePlayerState::Dead:
            handleDead();
            break;
        default:
            break;
        }
    }
    void PlayerScript::handleIdle()
    {
        // ->Move
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
            ChangeState(ePlayerState::Move);
    }
    void PlayerScript::handleMove()
    {        
        if (0 == mMoveCondition)
        {
            mCreatureRigidbody->SetMoving(false);
            mCreatureAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }
        else
            mCreatureRigidbody->SetMoving(true);
    }
    void PlayerScript::handleJump()
    {
        if (mCreatureFootCollider->IsGround())
            ChangeState(mpreviousState);
    }
    void PlayerScript::handleDead()
    {
        if (!mIsDead)
        {
            EffectScript* playerEffect = CreatureScript::callEffect();
            playerEffect->SetReverse(isLeft());
            todoActiveEffect(playerEffect, L"Dying");
            mWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
        }
        mIsDead = true;
        if (0 < mCreatureStat.CurHP)
        {
            mIsDead = false;
            ChangeState(ePlayerState::Idle);
            mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
        }
    }
#pragma endregion
#pragma region Weapon Logic
    void PlayerScript::inputAttack()
    {
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            // 비활성이라면 활성화 시켜줌
            if (GameObject::eObjectState::Active != mWeaponScript->GetOwner()->GetObjectState())
                mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
            mWeaponScript->ToDoAttack();
        }
    }
#pragma endregion
#pragma region Move Logic
    void PlayerScript::inputCameraMove()
    {
        Vector3 playerPos = mCreatureTransform->GetPosition();
        
        GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
    }
    void PlayerScript::inputMove()
    {
        // moveAnimation
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
            mMoveCondition++;
        if (Input::GetKeyUp(eKeyCode::D)
            || Input::GetKeyUp(eKeyCode::A))
            mMoveCondition--;


        if (mDashRunning)
            return;

        if (Input::GetKey(eKeyCode::D))
            CreatureScript::moveRight();

        if (Input::GetKey(eKeyCode::A))
            CreatureScript::moveLeft();


        if (mCreatureFootCollider->IsGround())
        {
            if (Input::GetKey(eKeyCode::S)
                && mCreatureFootCollider->IsPlatformCollision())
            {
                if (Input::GetKeyDown(eKeyCode::SPACE))
                    mCreatureFootCollider->ClearGroundBuffer();
            }
            else
            {
                // 버퍼에 추가
                if (Input::GetKeyDown(eKeyCode::SPACE))
                    mJumpData.BufferedJump = true;

                if (Input::GetKeyUp(eKeyCode::SPACE))
                {
                    // 버퍼 중단
                    mJumpData.BufferedJump = false;
                    // 바로 점프
                    todoJump();
                }
            }
        }
        else
        {
            // 점프 개수가 유효한지 확인
            if (mJumpData.ExtraJump)
            {
                if (Input::GetKeyDown(eKeyCode::SPACE))
                {
                    mJumpData.ExtraJump = false;
                    mJumpData.JumpForceRatio = 0.80f;
                    // 점프하기
                    todoJump();
                }
            }
        }

    }
    void PlayerScript::updateWalkEffect()
    {
        if (ePlayerState::Move != mActiveState)
            return;

        mDustAccumulateTime += (float)Time::DeltaTime();
        if (0.30f <= mDustAccumulateTime)
        {            
            todoDustSpawn();
            mDustAccumulateTime = 0.0f;
        }
    }
    void PlayerScript::todoDustSpawn()
    {
        EffectScript* playerEffect = CreatureScript::callEffect();
        playerEffect->SetReverse(isLeft());
        todoActiveEffect(playerEffect, L"DustEffect");
    }
#pragma endregion
#pragma region Jump & Dash Logic
        void PlayerScript::updateJumpRegen()
        {
            if (!mJumpData.ExtraJump)
                if (mCreatureFootCollider->IsGround())
                    mJumpData.ExtraJump = true;
        }
        void PlayerScript::updateDashRegen()
        {
            if (mDashData.MaxDashCount == mDashData.CurDashCount)
                return;

            mDashData.DashAccumulateTime += (float)Time::DeltaTime();
            if (mDashData.DashRegenTime <= mDashData.DashAccumulateTime)
            {
                if (mDashData.MaxDashCount > mDashData.CurDashCount)
                {
                    mDashData.DashAccumulateTime = 0.0f;
                    mDashData.CurDashCount++;
                }
            }
        }
        void PlayerScript::updatePassPlatformCheck()
        {
            math::Vector2 currentVelocity = mCreatureRigidbody->GetVelocity();

            // Dash상태가 아니고, y가 -인 경우에만
            if (!mDashRunning
                && 0 >= currentVelocity.y)
                mPassPlatform = false;
            else
                mPassPlatform = true;
        }
        void PlayerScript::inputDash()
        {
            if (Input::GetKeyDown(eKeyCode::RBTN))
            {
                // condition
                if (useDash())
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
            mCreatureRigidbody->OverrideVelocity(mCreatureDir, mDashData.DashForce);
            mHoldingDashTime = 0.0f;
            mDashRunning = true;
            mCreatureRigidbody->GravityAble(false);
            mPassPlatform = true;

            // 이게 있어야 바닥뚫고 대시 가능
            if (mCreatureFootCollider->IsPlatformCollision())
                mCreatureFootCollider->ClearGroundBuffer();

            //// 투사체 정보 갱신
            //mWeaponScript->ModifyProjectile(
            //    math::Vector2(0.80f, 1.20f), 0.0f, 0.30f, enums::eProjectileType::Body);
            //// 투사체 활성화
            //mWeaponScript->ActiveProjectile();
        }
        void PlayerScript::updateBufferedJump()
        {
            // 버퍼에 등록되면
            if (mJumpData.BufferedJump)
            {
                // 시간을 재다가
                mJumpData.JumpAccumulateTime += (float)Time::DeltaTime();

                // 조건에 맞으면 점프
                if (mJumpData.JumpLimitTime <= mJumpData.JumpAccumulateTime)
                {
                    mJumpData.JumpAccumulateTime = 0.0f;
                    mJumpData.JumpForceRatio = 1.0f;
                    todoJump();
                }
            }
        }
        void PlayerScript::updateDashValidTime()
        {
            if (mDashRunning)
            {
                mHoldingDashTime += (float)Time::DeltaTime();
                if (0.250f <= mHoldingDashTime)
                {
                    mHoldingDashTime = 0.0f;
                    mDashRunning = false;
                    mCreatureRigidbody->GravityAble(true);
                    mCreatureRigidbody->OverrideVelocity(math::Vector2::UnitY, 0.010f);
                }
            }
        }
        void PlayerScript::todoJump()
        {
            if (ePlayerState::Jump != mActiveState)
                ChangeState(ePlayerState::Jump);
            EffectScript* playerEffect = CreatureScript::callEffect();
            playerEffect->SetReverse(isLeft());
            todoActiveEffect(playerEffect, L"Jumping");

            // 최소 높이 설정
            float minForceRatio = 0.8750f;
            if (minForceRatio >= mJumpData.JumpForceRatio)
                mJumpData.JumpForceRatio = minForceRatio;

            mCreatureFootCollider->ClearGroundBuffer();

            if (mJumpData.ExtraJump)
                mCreatureRigidbody->ApplyVelocity(Vector2::UnitY, mJumpData.JumpForce * mJumpData.JumpForceRatio);
            else
                mCreatureRigidbody->OverrideVelocity(Vector2::UnitY, mJumpData.JumpForce * mJumpData.JumpForceRatio);
            clearJumpBuffer();
        }
#pragma endregion

#pragma region Initialize Player
    void PlayerScript::initializeAnimation()
    {
        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mCreatureAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerJump", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerDead", texture, Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->PlayAnimation(L"playerIdle");

        mCreatureAnimator->StartEvent(L"playerJump") = std::bind(&PlayerScript::StartJump, this);

    }
    void PlayerScript::initializeCollider()
    {
        // body
        {
            mCreatureBodyCollider->SetSize(Vector2(0.30f, 0.550f));
            mCreatureBodyCollider->SetCenter(Vector2(0.0f, -0.160f));
        }
        // foot
        {
            mCreatureFootCollider->SetSize(Vector2(0.150f, 0.050f));
            mCreatureFootCollider->SetCenter(Vector2(0.0f, -0.50f));
        }
    }
    void PlayerScript::initializeData()
    {
        mCreatureStat.MaxHP = 30;
        mCreatureStat.CurHP = mCreatureStat.MaxHP;
        mCreatureStat.MoveSpeed = 3.50f;

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

        //GameDataManager::SetPlayerStat(mCreatureStat);
        //GameDataManager::SetDashCount(mDashData);
        //GameDataManager::SetJumpCount(mJumpData);
    }
#pragma endregion
#pragma region Collision Func
    void PlayerScript::OnCollisionEnter(Collider2D* other)
    {
        // 몬스터의 시야에 닿은경우
        if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
            && Collider2D::eDetectionType::Sensor == other->GetDetectionType())
        {           
            GameObject* creatureObj = other->GetOwner();
            MonsterScript* creatureScript = creatureObj->GetComponent<MonsterScript>();
            creatureScript->MonsterFindsPlayer(true);
        }
        if (Collider2D::eDetectionType::Env == other->GetDetectionType()
            && mCreatureFootCollider->IsGround())
        {
            todoDustSpawn();
        }
    }
    void PlayerScript::OnCollisionExit(Collider2D* other)
    {
        // 플렛폼에 충돌
        if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
        {
            // 어디가 충돌했는지 확인하기
            if (mCreatureBodyCollider->IsCollision())
                mBodyCollision = false;
            if (mCreatureFootCollider->IsCollision())
                mFootCollision = false;
        }


        if (enums::eLayerType::Monster == other->GetOwner()->GetLayerType()
            && Collider2D::eDetectionType::Sensor == other->GetDetectionType())
        {
            GameObject* creatureObj = other->GetOwner();
            MonsterScript* creatureScript = creatureObj->GetComponent<MonsterScript>();
            creatureScript->MonsterFindsPlayer(false);
        }
    }
#pragma endregion
#pragma region public Func
    CombatScript* PlayerScript::SetWeaponObject(GameObject* object)
    {
        mWeaponScript = object->AddComponent<CombatScript>();
        mWeaponScript->AddPlayerObject(this);
        return mWeaponScript;
    }
#pragma endregion
}