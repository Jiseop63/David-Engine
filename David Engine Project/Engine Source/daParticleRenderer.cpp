#include "daParticleRenderer.h"
#include "daMesh.h"
#include "daMaterial.h"
#include "daResources.h"
#include "daTransform.h"
#include "daGameObject.h"
#include "daTime.h"

da::Particle particles[1000] = {}; // 파티클 개수?

namespace da
{

	ParticleRenderer::ParticleRenderer()
		: mParticleBuffer(nullptr)
		, mSharedBuffer(nullptr)
		, mCS(nullptr)
		, mCount(0)
		, mStartSize(math::Vector4::One)
		, mEndSize(math::Vector4::One)
		, mStartColor(math::Vector4::Zero)
		, mEndColor(math::Vector4::Zero)
		, mLifeTime(0.0f)
		, mFrequency(0.0f)
		, mTime(0.0f)
	{
		// 파티클 리소스 세팅
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(mesh);
		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		mCS = Resources::Find<ParticleShader>(L"ParticleComputeShader");

		// 구조화버퍼 세팅
		for (size_t i = 0; i < 1000; i++)
		{
			math::Vector4 pos = math::Vector4::Zero;
			pos.x += rand() % 20;
			pos.y += rand() % 10;

			int sign = rand() % 2;
			if (sign == 0)
				pos.x *= -1.0f;
			sign = rand() % 2;
			if (sign == 0)
				pos.y *= -1.0f;

			particles[i].Direction = math::Vector4( cosf((float)i * (XM_2PI / (float)1000)), sinf((float)i * (XM_2PI / 100.f)), 0.0f, 1.0f );
			particles[i].Position = pos;
			particles[i].Speed= 1.0f;
			particles[i].Active = 0;
		}
		mParticleBuffer = new da::graphics::StructuredBuffer();
		mParticleBuffer->Create(sizeof(Particle), 1000, eViewType::UAV, particles);

		mSharedBuffer = new da::graphics::StructuredBuffer();
		mSharedBuffer->Create(sizeof(Particle), 1, eViewType::UAV, nullptr, true);

		//mParticleBuffer->SetData(particles, 1000);
	}
	ParticleRenderer::~ParticleRenderer()
	{
	}
	void ParticleRenderer::Initialize()
	{
	}
	void ParticleRenderer::Update()
	{
	}
	void ParticleRenderer::LateUpdate()
	{
		float aliveTime = 1.0f / 1.0f;
		mTime += (float)Time::DeltaTime();

		if (mTime > aliveTime)
		{
			float f = (mTime / aliveTime);
			UINT AliveCount = (UINT)f;
			mTime = f - floor(f);

			ParticleShared shareData = {};
			shareData.SharedActiveCount = 2;
			mSharedBuffer->SetData(&shareData, 1);
		}
		else
		{
			ParticleShared shareData = {};
			shareData.SharedActiveCount = 0;
			mSharedBuffer->SetData(&shareData, 1);
		}


		mCS->SetParticleBuffer(mParticleBuffer);
		mCS->SetSharedBuffer(mSharedBuffer);
		mCS->OnExcute();
	}
	void ParticleRenderer::Render()
	{
		GetOwner()->GetTransform()->BindConstantBuffer();
		mParticleBuffer->BindSRV(eShaderStage::VS, 14);
		mParticleBuffer->BindSRV(eShaderStage::GS, 14);
		mParticleBuffer->BindSRV(eShaderStage::PS, 14);

		GetMaterial()->Binds();
		GetMesh()->RenderInstanced(1000);		// 파티클 개수? 를 넣는듯

		mParticleBuffer->Clear();
	}
}
