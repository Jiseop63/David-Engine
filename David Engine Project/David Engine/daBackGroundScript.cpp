#include "daBackGroundScript.h"

#include "daRenderer.h"
#include "daConstantBuffer.h"
#include "daTime.h"
#include "daInput.h"

namespace da
{
	using namespace math;

	BackGroundScript::BackGroundScript()
		: mAddTime(0.0f)
		, mSpeed(0.0f)
	{
	}
	BackGroundScript::~BackGroundScript()
	{
	}

	void BackGroundScript::Initialize()
	{
	}
	void BackGroundScript::Update()
	{
		// 테스트
		if (Input::GetKeyDown(eKeyCode::N_1))
		{
			mSpeed -= 0.3f;
		}
		if (Input::GetKeyDown(eKeyCode::N_2))
		{
			mSpeed += 0.3f;
		}
	}
	void BackGroundScript::LateUpdate()
	{
		// 매 틱마다 호출이 아니라, 렌더링 하는 타이밍에만 딱 한번 호출되게 변경해야 할듯?
		mAddTime += (float)Time::DeltaTime(); 
		renderer::TimeCB timeCB = {};
		timeCB.AddTime = mAddTime; 
		timeCB.Speed = mSpeed;
		graphics::ConstantBuffer* cb = renderer::constantBuffer[(UINT)graphics::eCBType::Time];
		cb->SetData(&timeCB);
		cb->Bind(graphics::eShaderStage::VS);
	}
	void BackGroundScript::Render()
	{
	}
}