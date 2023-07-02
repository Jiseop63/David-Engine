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
		// Å×½ºÆ®
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