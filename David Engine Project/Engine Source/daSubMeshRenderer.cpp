#include "daSubMeshRenderer.h"
#include "daGameObject.h"
#include "daTransform.h"
#include "daTimeConstants.h"
namespace da
{
	using namespace enums;

	SubMeshRenderer::SubMeshRenderer()
		: Component(eComponentType::SubRenderer)
		, mMesh(nullptr)
		, mMaterial(nullptr)
	{
	}
	SubMeshRenderer ::~SubMeshRenderer()
	{
	}

	void SubMeshRenderer::Render()
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
