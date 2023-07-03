#include "daMeshRenderer.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daTimeConstants.h"
namespace da
{
	using namespace enums;

	MeshRenderer::MeshRenderer()
		: Component(eComponentType::Renderer)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{
	}
	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::Initialize()
	{
	}
	void MeshRenderer::Update()
	{
	}
	void MeshRenderer::LateUpdate()
	{
	}
	void MeshRenderer::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();

		TimeConstants* timeConstants = GetOwner()->GetComponent<TimeConstants>();
		if (timeConstants)
			timeConstants->BindConstantBuffer();

		mMesh->BindBuffer();
		mMaterial->Binds();
		mMesh->Render();
		mMaterial->Clear();
	}
}
