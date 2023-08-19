#include "daParticleShader.h"
#include "daRenderer.h"
#include "daConstantBuffer.h"
#include "daTime.h"

namespace da::graphics
{
	ParticleShader::ParticleShader()
		: ComputeShader(128, 1, 1)
		, mParticleBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}
	ParticleShader::~ParticleShader()
	{
	}
	void ParticleShader::Binds()
	{
		mParticleBuffer->BindUAV(0);
		mSharedBuffer->BindUAV(1);

		mGroupX = mParticleBuffer->GetStride() / mThreadGroupCountX + 1;
		mGroupY = 1;
		mGroupZ = 1;
	}
	void ParticleShader::Clear()
	{
		mParticleBuffer->Clear();
		mSharedBuffer->Clear();
	}
	void ParticleShader::SetParticleBuffer(StructuredBuffer* particleBuffer)
	{
		mParticleBuffer = particleBuffer;
		ConstantBuffer* particleCB = renderer::constantBuffer[(UINT)eCBType::Particle];

		static float elapsedTime = 0.0f;
		elapsedTime += (float)Time::DeltaTime();
		renderer::ParticleCB particleData = {};
		particleData.ElementCount = mParticleBuffer->GetStride();
		particleData.ElpasedTime = (float)Time::DeltaTime();

		particleCB->SetData(&particleData);
		particleCB->Bind(eShaderStage::CS);
	}
}
