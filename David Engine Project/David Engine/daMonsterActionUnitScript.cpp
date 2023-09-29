#include "daMonsterActionUnitScript.h"
#include "daResources.h"

namespace da
{
	MonsterActionUnitScript::MonsterActionUnitScript()
	{
	}
	MonsterActionUnitScript::~MonsterActionUnitScript()
	{
	}
	void MonsterActionUnitScript::Initialize()
	{
		ActionUnitScript::Initialize();
		mActionUnitAnimator->Create(L"Dying", Resources::Find<Texture>(L"DyingFX"), math::Vector2::Zero, math::Vector2(64.0f, 64.0f), 12, math::Vector2::Zero, 0.050f, 40.0f);
		mActionUnitAnimator->CompleteEvent(L"Dying") = std::bind(&ActionUnitScript::ClearUnit, this);

		//mActionUnitAnimator->Create(L"BansheeBulletIdle", Resources::Find<Texture>(L"BansheeBulletIdle"), math::Vector2::Zero, math::Vector2(13.0f, 16.0f), 4, math::Vector2::Zero, 0.10f, 70.0f);
		//mActionUnitAnimator->Create(L"BansheeBulletHit", Resources::Find<Texture>(L"BansheeBulletHit"), math::Vector2::Zero, math::Vector2(20.0f, 27.0f), 6, math::Vector2::Zero, 0.10f, 80.0f);
		//// 종료 이벤트 넣어주기		
		//mActionUnitAnimator->CompleteEvent(L"BansheeBulletHit") = std::bind(&ActionUnitScript::ClearUnit, this);
	}
}
