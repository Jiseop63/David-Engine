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
        
    }
    void PlayerScript::MoveFunc(Vector2 dir)
    {
        mRigidbody->V2AddForce(dir * mMoveSpeed);
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