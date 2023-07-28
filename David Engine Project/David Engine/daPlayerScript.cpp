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
        , mRegenDashTime(1.750f)
        , mAddDashTime(0.0f)
        , mPlayerStat(nullptr)
        , mDashCount(nullptr)
        , mInventoryData(nullptr)
        , mMoveCondition(0)
        , mReverse(false)
        , mAngle(0.0f)
        , mAttacked(false)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
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
        regenDashCount();

        // Input
        GetInput();
        GetMouse();
        // FSM
        PlayerFSM();

    }
    void PlayerScript::GetInput()
    {
        //// move
        //{
        //    if (Input::GetKeyDown(eKeyCode::D)
        //        || Input::GetKeyDown(eKeyCode::A))
        //    {
        //        mAnimator->PlayAnimation(L"playerMove");
        //        mMoveCondition++;
        //    }
        //        
        //    if (Input::GetKeyUp(eKeyCode::D)
        //        || Input::GetKeyUp(eKeyCode::A))
        //    {
        //        mMoveCondition--;
        //        if (0 == mMoveCondition)
        //            mAnimator->PlayAnimation(L"playerIdle");
        //    }

        //    if (Input::GetKey(eKeyCode::D))
        //    {
        //        MoveFunc(Vector2::UnitX);
        //    }
        //    if (Input::GetKey(eKeyCode::A))
        //    {
        //        MoveFunc(-Vector2::UnitX);
        //    }
        //    if (Input::GetKey(eKeyCode::W))
        //    {
        //        MoveFunc(Vector2::UnitY);
        //    }
        //    if (Input::GetKey(eKeyCode::S))
        //    {
        //        MoveFunc(-Vector2::UnitY);
        //    }
        //    if (Input::GetKeyDown(eKeyCode::SPACE))
        //    {
        //        Jump();
        //    }
        //}
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
        /*if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            Dash();
        }
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            if (mAttacked)
                mAttacked = false;
            else
                mAttacked = true;
        }*/

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

        // �÷��̾� ����
        Vector2 playerDir(mousePosition.x - playerPosition.x, mousePosition.y - playerPosition.y);        
        playerDir.Normalize();
        mPlayerDir = playerDir;

        // ���� ��ġ
        Vector3 weaponPosition(playerPosition.x, playerPosition.y, 0.0f);
        // ���� ȸ����
        mAngle = atan2(playerDir.y, playerDir.x);
        // ���� ��ġ ����
        mWeaponTransform->SetPosition(weaponPosition);
        // ���� ȸ�� ����
        mWeaponTransform->SetRotation(Vector3(0.0f, 0.0f, mAngle));

        // ��ġ�� ���� �¿� ����
        if (0 <= playerDir.x)
        {
            mRenderer->SetReverse(false);
            mWeaponRenderer->SetReverse(false);
            if (mAttacked)
            {
                // �̹��� ����
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
                // �̹��� ����
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword1Texture"));
            }
            else
            {
                mWeaponRenderer->ChangeSlotTexture(Resources::Find<Texture>(L"GreatSword0Texture"));
            }
        }
        
    }

    void PlayerScript::MoveFunc(Vector2 dir)
    {
        mRigidbody->ApplyForce(dir, mPlayerStat->MoveSpeed);
    }

    void PlayerScript::Dash()
    {
        // condition
        if (GameDataManager::UseDash())
        {
            // to do
            mFootCollider->ApplyGround(false);
            mRigidbody->ApplyVelocity(mPlayerDir, mPlayerStat->MoveSpeed * 2.0f);
        }
    }
    void PlayerScript::Jump()
    {
        mFootCollider->ApplyGround(false);
        mRigidbody->ApplyVelocity(Vector2::UnitY, mPlayerStat->MoveSpeed);
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
        case da::ePlayerState::Attack:
            HandleAttack();
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
        mPrevState = mActiveState;
        mActiveState = state;
    }

    void PlayerScript::HandleIdle()
    {
        // ����
        // ->Move
        // Ű�� �������� KeyDonw ���� �ٲ��ֱ�
        if (Input::GetKeyDown(eKeyCode::D)
            || Input::GetKeyDown(eKeyCode::A))
        {
            ChangeState(ePlayerState::Move);
        }

        // ->Jump
        if (Input::GetKeyDown(eKeyCode::SPACE))
        {
            mAnimator->PlayAnimation(L"playerJump");
            Jump();
            ChangeState(ePlayerState::Jump);
        }
        // ->Dash
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            mAnimator->PlayAnimation(L"playerJump");
            Dash();
            ChangeState(ePlayerState::Jump);
        }
        // ->Attack
    }
    void PlayerScript::HandleMove()
    {
        // Todo
        // �ִϸ��̼� �� ���� ����
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
        }
        // ������ ����
        if (Input::GetKey(eKeyCode::D))
        {
            MoveFunc(Vector2::UnitX);
        }
        if (Input::GetKey(eKeyCode::A))
        {
            MoveFunc(-Vector2::UnitX);
        }

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
            Jump();
            ChangeState(ePlayerState::Jump);
        }
        // ->Dash
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            mAnimator->PlayAnimation(L"playerJump");
            Dash();
            ChangeState(ePlayerState::Jump);
        }

        // ->Attack
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            ChangeState(ePlayerState::Attack);
        }
    }
    void PlayerScript::HandleJump()
    {
        // ������ ����
        if (Input::GetKey(eKeyCode::D))
        {
            MoveFunc(Vector2::UnitX);
        }
        if (Input::GetKey(eKeyCode::A))
        {
            MoveFunc(-Vector2::UnitX);
        }
        // ->Dash
        if (Input::GetKeyDown(eKeyCode::RBTN))
        {
            mAnimator->PlayAnimation(L"playerJump");
            Dash();
        }


        // ->Idle
        if (mFootCollider->IsGround())
        {
            mAnimator->PlayAnimation(L"playerIdle");
            ChangeState(ePlayerState::Idle);
        }
       
        // ->Attack
        if (Input::GetKeyDown(eKeyCode::LBTN))
        {
            ChangeState(ePlayerState::Attack);
        }
    }
    void PlayerScript::HandleAttack()
    {
        // todo
        // GetMouse�Լ����� ����
        {
            if (mAttacked)
                mAttacked = false;
            else
                mAttacked = true;
        }
        {
            mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Default);
        }
        mAddAttackDelayTime += (float)Time::DeltaTime();
        if (0.80f <= mAddAttackDelayTime)
        {
            mWeaponCollider->SetColliderDetection(Collider2D::eColliderDetection::Inactive);
            mAddAttackDelayTime = 0.0f;
            ChangeState(mPrevState);
        }
        // ret ������ �ܺο��� �ؾ��ҰŰ����� �ӽ÷� �����ұ�?

        //// ->Jump
        //if (Input::GetKeyDown(eKeyCode::SPACE))
        //{
        //    mAnimator->PlayAnimation(L"playerJump");
        //    Jump();
        //    ChangeState(ePlayerState::Jump);
        //}
        //// ->Dash
        //if (Input::GetKeyDown(eKeyCode::RBTN))
        //{
        //    mAnimator->PlayAnimation(L"playerJump");
        //    Dash();
        //    ChangeState(ePlayerState::Jump);
        //}


    }
    void PlayerScript::HandleDead()
    {

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
    void PlayerScript::regenDashCount()
    {
        if (mDashCount->MaxCount == mDashCount->CurCount)
            return;

        mAddDashTime += (float)Time::DeltaTime();
        if (mRegenDashTime <= mAddDashTime)
        {
            GameDataManager::RecoveryDash();
            mAddDashTime = 0.0f;
        }
    }

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
    void PlayerScript::OnLandStay(Collider2D* other)
    {
        // Land Pos, Size ��������
        Vector3 landPos = other->GetTotalPosition();
        Vector3 landSize = other->GetTotalScale();
        // �� Pos, Size ��������
        Vector3 myPos = mFootCollider->GetTotalPosition();
        Vector3 mySize = mFootCollider->GetTotalScale();

        // A ������ �ּ�/�ִ� ��ġ
        Vector3 minLand = landPos - landSize * 0.5f;
        Vector3 maxLand = landPos + landSize * 0.5f;

        // B ������ �ּ�/�ִ� ��ġ
        Vector3 minMe = myPos - mySize * 0.5f;
        Vector3 maxMe = myPos + mySize * 0.5f;

        // ��Ȳ�� �÷��̾ �ٴڿ� �İ�����
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
}