#include "daPlayerScript.h"
#include "daGameObject.h"
#include "daInput.h"
#include "daGameDataManager.h"
namespace da
{
	PlayerScript::PlayerScript()
		: mRigidbody(nullptr)
        , mMoveSpeed(6.0f)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
        mRigidbody = GetOwner()->GetComponent<Rigidbody>();
	}
    void PlayerScript::Update()
    {
        GetInput();
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
        mRigidbody->ApplyV2Velocity(dir * mMoveSpeed);
    }
    void PlayerScript::Dash()
    {
        // 마우스 방향 구하기
        Vector3 mousePos = Input::GetMouseWorldPosition();
        Vector2 calcVector(mousePos.x, mousePos.y);
        calcVector.Normalize();
        // 이동 거리 적용하기
        float dash = 20.0f;
        calcVector *= dash;
        mRigidbody->ApplyV2Force(calcVector);
    }
    void PlayerScript::Jump()
    {
    }
    void PlayerScript::GetDamage()
    {
        float value = 5.0f;
        structs::eCreatureStat& myStat = GameDataManager::GetPlayerStat();
        myStat.curHP -= value;
        if (0 >= myStat.curHP)
            myStat.curHP = 0;
    }

    void PlayerScript::GetHeal()
    {
        float value = 5.0f;
        structs::eCreatureStat& myStat = GameDataManager::GetPlayerStat();
        myStat.curHP += value;
        
        if (myStat.maxHP <= myStat.curHP)
            myStat.curHP = myStat.maxHP;
    }
}