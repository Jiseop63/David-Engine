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
		renderer::TimeCB cbData = {};
		cbData.ElapsedTime = mElapsedTime;
		cbData.Value = mValue;

		graphics::ConstantBuffer* timeCB = renderer::constantBuffer[(UINT)graphics::eCBType::Time];
		timeCB->SetData(&cbData);
		timeCB->Bind(graphics::eShaderStage::VS);
	}
}
