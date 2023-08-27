#pragma once
#include "daMeshRenderer.h"
#include "daStructuredBuffer.h"
#include "daParticleShader.h"

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
		graphics::StructuredBuffer*		mParticleBuffer;
		graphics::StructuredBuffer*		mSharedBuffer;

		std::shared_ptr<ParticleShader> mCS;
		UINT							mCount;
		math::Vector4					mStartSize;
		math::Vector4					mEndSize;
		math::Vector4					mStartColor;
		math::Vector4					mEndColor;
		float							mLifeTime;
		float							mFrequency;
		float							mTime;
	};
}