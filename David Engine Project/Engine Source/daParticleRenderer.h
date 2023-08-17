#pragma once
#include "daMeshRenderer.h"
#include "daStructuredBuffer.h"

namespace da
{
	class ParticleRenderer : public MeshRenderer
	{
	public:
		ParticleRenderer();
		virtual ~ParticleRenderer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	private:
		graphics::StructuredBuffer* mParticleBuffer;
		UINT						mCount;
		math::Vector4				mStartSize;
		math::Vector4				mEndSize;
		math::Vector4				mStartColor;
		math::Vector4				mEndColor;
		float						mLifeTime;

	};
}