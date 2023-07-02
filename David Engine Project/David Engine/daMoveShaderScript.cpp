#include "daMoveShaderScript.h"
#include "daRenderer.h"
#include "daTime.h"

namespace da
{
	MoveShaderScript::MoveShaderScript()
		: mAddTime(0.0f)
		, mSpeed(0.0f)
	{
	}
	MoveShaderScript::~MoveShaderScript()
	{
	}
	void MoveShaderScript::Initialize()
	{
	}
	void MoveShaderScript::Update()
	{
	}
	void MoveShaderScript::LateUpdate()
	{
		mAddTime += (float)Time::DeltaTime();
		renderer::TimeCB timeCB = {};
		timeCB.AddTime = mAddTime;
		timeCB.Speed = mSpeed;
		graphics::ConstantBuffer* cb = renderer::constantBuffer[(UINT)graphics::eCBType::Time];
		cb->SetData(&timeCB);
		cb->Bind(graphics::eShaderStage::VS);
	}
	void MoveShaderScript::Render()
	{
	}
}
