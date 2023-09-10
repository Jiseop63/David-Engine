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

        , mJumpData(nullptr)
        , mDashData(nullptr)

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

        InitAnimation();
        InitCollider();

        mCreatureStat = &GameDataManager::GetPlayerStat();
        mJumpData = &GameDataManager::GetJumpCount();
        mDashData = &GameDataManager::GetDashCount();
	}
    void PlayerScript::Update()
    {
        PlayerCondition();
        timeProcess();
        PlayerInput();
        GetMouse();
        PlayerFSM();
    }
#pragma endregion
#pragma region Common Func
    void PlayerScript::PlayerCondition()
    {        
        if (0 >= mCreatureStat->CurHP)
            ChangeState(ePlayerState::Dead);
    }
    void PlayerScript::PlayerInput()
    {
        DebugInput();
        UIInput();
        if (mIsDead)
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
            GetHeal(); 
        if (Input::GetKeyDown(eKeyCode::T))
            GetDamage();
        if (Input::GetKeyDown(eKeyCode::Y))
        {
            GameDataManager::DebugMode();
            bool isDebug = GameDataManager::IsDebuging();
            int a = 0;
        }
    }
    void PlayerScript::UIInput()
    {
        if (Input::GetKeyDown(eKeyCode::V))
            GameDataManager::CallInventory();
        if (Input::GetKeyDown(eKeyCode::TILDE))
            GameDataManager::ChangeArmour();
    }
    void PlayerScript::GetMouse()
    {
        if (mIsDead)
            return;
        
        CalcPlayerDir();
    }
    void PlayerScript::CalcPlayerDir()
    {
        Vector3 mouseWorldPosition = Input::GetMouseWorldPosition();
        Vector2 mousePosition(mouseWorldPosition.x, mouseWorldPosition.y);
        Vector3 playerPosition = mCreatureTransform->GetPosition();
        // Player Dir
        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);
        playerDir.Normalize();
        mCreatureDir = playerDir;

        bool value = IsLeft();
        mCreatureRenderer->SetReverse(value);
    }
    void PlayerScript::timeProcess()
    {
        dashRegen();
        jumpRegen();

        bufferedJump(); // �����Ŀ� ������
        walkEffect();     
        endJumping();   // �̰� �־�� �÷��� �������� �ȸ���

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

    EffectScript* PlayerScript::callEffect()
    {
        for (size_t effect = 0; effect < mEffects.size(); effect++)
        {
            if (GameObject::eObjectState::Inactive ==
                mEffects[effect]->GetOwner()->GetObjectState())
                return mEffects[effect];
        }
        return nullptr;
    }
    void PlayerScript::activeEffect(EffectScript* effect, const std::wstring name)
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
            mCreatureRigidbody->SetMoving(false);
            mCreatureAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }
        else
            mCreatureRigidbody->SetMoving(true);
    }
    void PlayerScript::HandleJump()
    {
        if (mCreatureFootCollider->IsGround())
            ChangeState(mpreviousState);
    }
    void PlayerScript::HandleDead()
    {
        if (!mIsDead)
        {
            EffectScript* playerEffect = callEffect();
            playerEffect->SetReverse(IsLeft());
            activeEffect(playerEffect, L"Dying");
            mWeaponScript->GetOwner()->SetObjectStates(GameObject::eObjectState::Inactive);
        }
        mIsDead = true;
        if (0 < mCreatureStat->CurHP)
        {
            mIsDead = false;
            ChangeState(ePlayerState::Idle);
            mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
        }
    }
#pragma endregion
#pragma region Weapon Logic
    void PlayerScript::InputAttack()
    {
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            // ��Ȱ���̶�� Ȱ��ȭ ������
            if (GameObject::eObjectState::Active != mWeaponScript->GetOwner()->GetObjectState())
                mWeaponScript->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
            mWeaponScript->ToDoAttack();
        }
    }
#pragma endregion
#pragma region Move Logic
    void PlayerScript::CameraMove()
    {
        Vector3 playerPos = mCreatureTransform->GetPosition();
        
        GameDataManager::SetCameraMovaPosition(math::Vector2(playerPos.x, playerPos.y));
    }
    void PlayerScript::InputMove()
    {        
        Vector3 mPos = mCreatureTransform->GetPosition();
        float moveMagnitude = mCreatureStat->MoveSpeed * (float)Time::DeltaTime();

        // moveAnimation
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
            mMoveCondition++;
        if (Input::GetKeyUp(eKeyCode::D)
            || Input::GetKeyUp(eKeyCode::A))
            mMoveCondition--;

        Collider2D::eWallCollisionState wallCollisionState = mCreatureBodyCollider->IsWallCollision();

        if (mDashRunning)
            return;

        if (Input::GetKey(eKeyCode::D))
        {        
            if (Collider2D::eWallCollisionState::Right == wallCollisionState)
                return;
            else
            {
                Vector2 moveDir = daRotateVector2(Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
                Vector2 movePosition = moveDir * moveMagnitude;

                mPos.x += movePosition.x;
                mPos.y += movePosition.y;
            }
        }
        if (Input::GetKey(eKeyCode::A))
        {
            if (Collider2D::eWallCollisionState::Left == wallCollisionState)
                return;
            else
            {
                Vector2 moveDir = daRotateVector2(-Vector2::UnitX, mCreatureFootCollider->GetEnvRotate());
                Vector2 movePosition = moveDir * moveMagnitude;
                                
                mPos.x += movePosition.x;
                mPos.y += movePosition.y;
            }
        }

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
                // ���ۿ� �߰�
                if (Input::GetKeyDown(eKeyCode::SPACE))
                    mJumpData->BufferedJump = true;

                if (Input::GetKeyUp(eKeyCode::SPACE))
                {
                    // ���� �ߴ�
                    mJumpData->BufferedJump = false;
                    // �ٷ� ����
                    todoJump();
                }
            }
        }
        else
        {
            // ���� ������ ��ȿ���� Ȯ��
            if (mJumpData->ExtraJump)
            {
                if (Input::GetKeyDown(eKeyCode::SPACE))
                {
                    mJumpData->ExtraJump = false;
                    mJumpData->JumpForceRatio = 0.80f;
                    // �����ϱ�
                    todoJump();
                }
            }
        }

        mCreatureTransform->SetPosition(mPos);
    }
    void PlayerScript::walkEffect()
    {
        if (ePlayerState::Move != mActiveState)
            return;

        mDustAccumulateTime += (float)Time::DeltaTime();
        if (0.30f <= mDustAccumulateTime)
        {            
            dustSpawn();
            mDustAccumulateTime = 0.0f;
        }
    }
    void PlayerScript::dustSpawn()
    {
        EffectScript* playerEffect = callEffect();
        playerEffect->SetReverse(IsLeft());
        activeEffect(playerEffect, L"DustEffect");
    }
#pragma endregion
#pragma region Jump & Dash Logic
        void PlayerScript::jumpRegen()
        {
            if (!mJumpData->ExtraJump)
                if (mCreatureFootCollider->IsGround())
                    mJumpData->ExtraJump = true;
        }
        void PlayerScript::dashRegen()
        {
            if (mDashData->MaxDashCount == mDashData->CurDashCount)
                return;

            mDashData->DashAccumulateTime += (float)Time::DeltaTime();
            if (mDashData->DashRegenTime <= mDashData->DashAccumulateTime)
                GameDataManager::RecoveryDash();
        }
        void PlayerScript::endJumping()
        {
            math::Vector2 currentVelocity = mCreatureRigidbody->GetVelocity();

            // Dash���°� �ƴϰ�, y�� -�� ��쿡��
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
            // �̵���Ű��
            mCreatureRigidbody->OverrideVelocity(mCreatureDir, mDashData->DashForce);
            mHoldingDashTime = 0.0f;
            mDashRunning = true;
            mCreatureRigidbody->GravityAble(false);
            mPassPlatform = true;

            // �̰� �־�� �ٴڶհ� ��� ����
            if (mCreatureFootCollider->IsPlatformCollision())
                mCreatureFootCollider->ClearGroundBuffer();

            //// ����ü ���� ����
            //mWeaponScript->ModifyProjectile(
            //    math::Vector2(0.80f, 1.20f), 0.0f, 0.30f, enums::eProjectileType::Body);
            //// ����ü Ȱ��ȭ
            //mWeaponScript->ActiveProjectile();
        }
        void PlayerScript::inputJump()
        {
            

        }
        void PlayerScript::bufferedJump()
        {
            // ���ۿ� ��ϵǸ�
            if (mJumpData->BufferedJump)
            {
                // �ð��� ��ٰ�
                mJumpData->JumpAccumulateTime += (float)Time::DeltaTime();

                // ���ǿ� ������ ����
                if (mJumpData->JumpLimitTime <= mJumpData->JumpAccumulateTime)
                {
                    mJumpData->JumpAccumulateTime = 0.0f;
                    mJumpData->JumpForceRatio = 1.0f;
                    todoJump();
                }
            }
        }
        void PlayerScript::todoJump()
        {
            if (ePlayerState::Jump != mActiveState)
                ChangeState(ePlayerState::Jump);
            EffectScript* playerEffect = callEffect();
            playerEffect->SetReverse(IsLeft());
            activeEffect(playerEffect, L"Jumping");

            // �ּ� ���� ����
            float minForceRatio = 0.8750f;
            if (minForceRatio >= mJumpData->JumpForceRatio)
                mJumpData->JumpForceRatio = minForceRatio;

            mCreatureFootCollider->ClearGroundBuffer();

            if (mJumpData->ExtraJump)
                mCreatureRigidbody->ApplyVelocity(Vector2::UnitY, mJumpData->JumpForce * mJumpData->JumpForceRatio);
            else
                mCreatureRigidbody->OverrideVelocity(Vector2::UnitY, mJumpData->JumpForce * mJumpData->JumpForceRatio);
            GameDataManager::ClearJumpBuffer();
        }
#pragma endregion

#pragma region Initialize Player
    void PlayerScript::InitAnimation()
    {
        std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"PlayerSprite", L"..\\Resources\\Texture\\Adventurer\\SpriteSheet.png");
        mCreatureAnimator->Create(L"playerIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 5, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerMove", texture, Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f), 8, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerJump", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->Create(L"playerDead", texture, Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
        mCreatureAnimator->PlayAnimation(L"playerIdle");

        mCreatureAnimator->StartEvent(L"playerJump") = std::bind(&PlayerScript::StartJump, this);

    }
    void PlayerScript::InitCollider()
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
#pragma endregion
#pragma region Collision Func
    void PlayerScript::OnCollisionEnter(Collider2D* other)
    {
        // ������ �þ߿� �������
        if (enums::eLayerType::Creature == other->GetOwner()->GetLayerType()
            && Collider2D::eDetectionType::Sensor == other->GetDetectionType())
        {           
            GameObject* creatureObj = other->GetOwner();
            MonsterScript* creatureScript = creatureObj->GetComponent<MonsterScript>();
            creatureScript->MonsterFindsPlayer(true);
        }
        if (Collider2D::eDetectionType::Env == other->GetDetectionType()
            && mCreatureFootCollider->IsGround())
        {
            dustSpawn();
        }
    }
    void PlayerScript::OnCollisionExit(Collider2D* other)
    {
        // �÷����� �浹
        if (enums::eLayerType::Platform == other->GetOwner()->GetLayerType())
        {
            // ��� �浹�ߴ��� Ȯ���ϱ�
            if (mCreatureBodyCollider->IsCollision())
                mBodyCollision = false;
            if (mCreatureFootCollider->IsCollision())
                mFootCollision = false;
        }


        if (enums::eLayerType::Creature == other->GetOwner()->GetLayerType()
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
        mWeaponScript->SetPlayerScript(this);
        return mWeaponScript;
    }
#pragma endregion
}