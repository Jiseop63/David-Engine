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
        , mWeaponCollider(nullptr)
        , mWeaponObject(nullptr)
        , mWeaponTransform(nullptr)
        , mWeaponRenderer(nullptr)
        , mWeaponScript(nullptr)
        , mPlayerDir(math::Vector2::Zero)
        , mDashRegenTime()
        , mDashAccumulateTime(0.0f)
        , mPlayerStat(nullptr)
        , mDashCount(nullptr)
        , mInventoryData(nullptr)
        , mMoveCondition(0)
        , mAttacked(false)
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
        mDashCount = &GameDataManager::GetDashCount();
        mInventoryData = &GameDataManager::GetInventory();
	}
    void PlayerScript::Update()
    {
        // Time
        timeProcess();

        // Input
        GetInput();
        GetMouse();

        // FSM
        PlayerFSM();

    }
#pragma endregion
#pragma region ETC Input Func
    void PlayerScript::GetInput()
    {        
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
        // inventory
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
        Vector3 mouseWorldPosition = Input::GetMouseWorldPosition();
        Vector2 mousePosition(mouseWorldPosition.x, mouseWorldPosition.y);
        Vector3 playerPosition = mTransform->GetPosition();

        // 플레이어 방향
        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);        
        playerDir.Normalize();
        mPlayerDir = playerDir;

        // 무기 위치
        Vector3 weaponPosition(playerPosition.x, playerPosition.y, 0.0f);
        // 무기 회전값
        float angle = atan2(mPlayerDir.y, mPlayerDir.x);
        // 무기 위치 변경
        mWeaponTransform->SetPosition(weaponPosition);
        // 무기 회전 적용
        mWeaponTransform->SetRotation(Vector3(0.0f, 0.0f, angle));

        // 위치에 따른 좌우 반전
        if (0 <= mPlayerDir.x)
        {
            mRenderer->SetReverse(false);
            mWeaponRenderer->SetReverse(false);
            if (mAttacked)
            {
                // 이미지 변경
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword1Texture"));
            }
            else
            {
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword0Texture"));
            }

        }
        else
        {
            mRenderer->SetReverse(true);
            mWeaponRenderer->SetReverse(true);
            if (mAttacked)
            {
                // 이미지 변경
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword1Texture"));
            }
            else
            {
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword0Texture"));
            }
        }
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
        case da::ePlayerState::todoJump:
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
        // 조건
        // ->Move
        // 키를 눌렀을때 KeyDonw 상태 바꿔주기
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
        {
            mMoveCondition++;
            mAnimator->PlayAnimation(L"playerMove");
            ChangeState(ePlayerState::Move);
        }

        // ->Jump
        if (Input::GetKeyDown(eKeyCode::SPACE))
        {
            mAnimator->PlayAnimation(L"playerJump");
            todoJump();
            ChangeState(ePlayerState::todoJump);
        }
        // ->Dash
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            mAnimator->PlayAnimation(L"playerJump");
            todoDash();
            ChangeState(ePlayerState::todoJump);
        }
        if (Input::GetKey(eKeyCode::LBTN))
        {
            todoAttack();
        }
    }
    void PlayerScript::HandleMove()
    {
        // Todo
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
        {
            mMoveCondition++;
        }
        if (Input::GetKeyUp(eKeyCode::D)
            || Input::GetKeyUp(eKeyCode::A))
        {
            mMoveCondition--;
        }
        todoMove();


        // ->Idle
        if (0 == mMoveCondition)
        {
            mAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }

        // ->Jump
        if (Input::GetKeyDown(eKeyCode::SPACE))
        {
            mAnimator->PlayAnimation(L"playerJump");
            todoJump();
            ChangeState(ePlayerState::todoJump);
        }
        // ->Dash
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            mAnimator->PlayAnimation(L"playerJump");
            todoDash();
            ChangeState(ePlayerState::todoJump);
        }

        if (Input::GetKey(eKeyCode::LBTN))
        {
            todoAttack();
        }
    }
    void PlayerScript::HandleJump()
    {
        todoMove();
                
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            todoDash();
        }

        // ->Idle
        if (mFootCollider->IsGround())
        {
            mAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }

        if (Input::GetKey(eKeyCode::LBTN))
        {
            todoAttack();
        }
    }
    void PlayerScript::HandleDead()
    {

    }
#pragma endregion
#pragma region Common Func
    void PlayerScript::todoMove()
    {
        if (Input::GetKey(eKeyCode::D))
        {
            mRigidbody->ApplyForce(Vector2::UnitX, mPlayerStat->MoveSpeed);
        }
        if (Input::GetKey(eKeyCode::A))
        {
            mRigidbody->ApplyForce(-Vector2::UnitX, mPlayerStat->MoveSpeed);
        }
    }
    void PlayerScript::todoJump()
    {
        mFootCollider->ApplyGround(false);
        mRigidbody->ApplyVelocity(Vector2::UnitY, mPlayerStat->MoveSpeed);
    }
    void PlayerScript::todoDash()
    {
        // condition
        if (GameDataManager::UseDash())
        {
            // to do
            mFootCollider->ApplyGround(false);
            mRigidbody->ApplyVelocity(mPlayerDir, mPlayerStat->MoveSpeed * 2.0f);
        }
    }
    void PlayerScript::todoAttack()
    {        
        // 공격 가능
        if (Collider2D::eColliderDetection::Inactive == mWeaponCollider->GetColliderDetection())
        {
            // 모션 바꿔주기
            if (mAttacked)
                mAttacked = false;
            else
                mAttacked = true;
            // 공격 활성화 하기 (나중에 함수로 WeaponScript에 요청하기관리)
            mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Default);
            mWeaponCollider->LateUpdate();
        }
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
        attackDelay();
        
    }
    void PlayerScript::dashRegen()
    {
        if (mDashCount->MaxCount == mDashCount->CurCount)
            return;

        mDashAccumulateTime += (float)Time::DeltaTime();
        if (mDashRegenTime <= mDashAccumulateTime)
        {
            GameDataManager::RecoveryDash();
            mDashAccumulateTime = 0.0f;
        }
    }
    void PlayerScript::attackDelay()
    {
        if (Collider2D::eColliderDetection::Default == mWeaponCollider->GetColliderDetection())
        {
            mAttackAccumulateTime += (float)Time::DeltaTime();
            float attackDelay = 0.80f;

            if (attackDelay <= mAttackAccumulateTime)
            {
                mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);
                mWeaponCollider->LateUpdate();
                mAttackAccumulateTime = 0.0f;
            }
        }
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
    void PlayerScript::SetWeaponObject(GameObject* object)
    {
        mWeaponObject = object;
        mWeaponTransform = mWeaponObject->GetComponent<Transform>();
        mWeaponRenderer = mWeaponObject->GetComponent<MeshRenderer>();
        mWeaponCollider = mWeaponObject->AddComponent<Collider2D>();
        mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);
        mWeaponScript = mWeaponObject->AddComponent<WeaponScript>();
        mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword0Texture"));
        // 9 22
        //mWeaponTransform->SetScale(Vector3(0.180f, 0.440f, 1.0f));
        mWeaponTransform->SetScale(Vector3(2.0f, 2.0f, 1.0f));
    }
#pragma endregion
#pragma region Collision Func
    void PlayerScript::OnLandStay(Collider2D* other)
    {
        // Land Pos, Size 가져오기
        Vector3 landPos = other->GetTotalPosition();
        Vector3 landSize = other->GetTotalScale();
        // 내 Pos, Size 가져오기
        Vector3 myPos = mFootCollider->GetTotalPosition();
        Vector3 mySize = mFootCollider->GetTotalScale();

        // A 상자의 최소/최대 위치
        Vector3 minLand = landPos - landSize * 0.5f;
        Vector3 maxLand = landPos + landSize * 0.5f;

        // B 상자의 최소/최대 위치
        Vector3 minMe = myPos - mySize * 0.5f;
        Vector3 maxMe = myPos + mySize * 0.5f;

        // 정황상 플레이어가 바닥에 파고들었음
        if (minLand.x <= maxMe.x && maxLand.x >= minMe.x &&
            minLand.y <= maxMe.y && maxLand.y >= minMe.y &&
            minLand.z <= maxMe.z && maxLand.z >= minMe.z)
        {
            float landHeight = maxLand.y - minLand.y;
            float myHeight = maxMe.y - minMe.y;
            float totalHeight = landHeight / 2.0f + myHeight / 2.0f;

            float collisionDepth = myPos.y - landPos.y;
            float pushDistance = 0.0f;
            if (collisionDepth < totalHeight)
            {
                pushDistance = totalHeight - collisionDepth;
            }
            Vector3 newPosition = mTransform->GetPosition();
            newPosition.y += pushDistance;
            mTransform->SetPosition(newPosition);            
        }
    }
#pragma endregion
}