#include "daParticleRenderer.h"
#include "daMesh.h"
#include "daMaterial.h"
#include "daResources.h"
#include "daTransform.h"
#include "daGameObject.h"


namespace da
{
	ParticleRenderer::ParticleRenderer()
		: mParticleBuffer{}
		, mCount(0)
		, mStartSize(math::Vector4::One)
		, mEndSize(math::Vector4::One)
		, mStartColor(math::Vector4::Zero)
		, mEndColor(math::Vector4::Zero)
		, mLifeTime(0.0f)
	{
		// ��ƼŬ ���ҽ� ����
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(mesh);
		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		// ����ȭ���� ����
		Particle particles[1000] = {}; // ��ƼŬ ����?
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

			particles[i].Position = pos;
			particles[i].Active = 1;
		}
		mParticleBuffer = new da::graphics::StructuredBuffer();
		mParticleBuffer->Create(sizeof(Particle), 1000, eSRVType::None);
		mParticleBuffer->SetData(particles, 1000);
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
	}
	void ParticleRenderer::Render()
	{
		GetOwner()->GetTransform()->BindConstantBuffer();
		mParticleBuffer->Bind(eShaderStage::VS, 14);
		mParticleBuffer->Bind(eShaderStage::GS, 14);
		mParticleBuffer->Bind(eShaderStage::PS, 14);

		GetMaterial()->Binds();
		GetMesh()->RenderInstanced(1000);		// ��ƼŬ ����? �� �ִµ�
	}
}
