#include "daTimeConstants.h"
#include "daTime.h"
#include "daConstantBuffer.h"
#include "daRenderer.h"

namespace da
{
	TimeConstants::TimeConstants()
		: Component(enums::eComponentType::TimeConstants)
		, mElapsedTime(0.0f)
		, mValue(0.0f)
	{
	}
	TimeConstants::~TimeConstants()
	{
	}
	void TimeConstants::BindConstantBuffer()
	{
		mElapsedTime += (float)Time::DeltaTime();
		renderer::TimeCB timeCB = {};
		timeCB.ElapsedTime = mElapsedTime;
		timeCB.Value = mValue;
		graphics::ConstantBuffer* cb = renderer::constantBuffer[(UINT)graphics::eCBType::Time];
		cb->SetData(&timeCB);
		cb->Bind(graphics::eShaderStage::VS);
	}
}
