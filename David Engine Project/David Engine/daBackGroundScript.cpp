#include "daBackGroundScript.h"

#include "daRenderer.h"
#include "daConstantBuffer.h"
#include "daTime.h"

namespace da
{
	using namespace math;

	BackGroundScript::BackGroundScript()
		: mAddTime(0.0f)
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
	}
	void BackGroundScript::LateUpdate()
	{
		mAddTime += Time::DeltaTime(); 
		renderer::TimeCB timeCB = {};
		timeCB.AddTime = mAddTime; 

		graphics::ConstantBuffer* cb = renderer::constantBuffer[(UINT)graphics::eCBType::Time];
		cb->SetData(&timeCB);
		cb->Bind(graphics::eShaderStage::VS); 
	}
	void BackGroundScript::Render()
	{
	}
}