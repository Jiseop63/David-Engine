#pragma once	
#include "daComputeShader.h"
#include "daStructuredBuffer.h"

namespace da::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		virtual ~ParticleShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetParticleBuffer(StructuredBuffer* particleBuffer);
		void SetSharedBuffer(StructuredBuffer* sharedBuffer) { mSharedBuffer = sharedBuffer; }

	private:
		StructuredBuffer* mParticleBuffer;
		StructuredBuffer* mSharedBuffer;
	};
}