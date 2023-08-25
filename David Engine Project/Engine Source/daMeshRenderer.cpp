#include "daMeshRenderer.h"
#include "daGameObject.h"
#include "daRenderer.h"
#include "daTransform.h"
#include "daTimeConstants.h"
#include "daAnimator.h"
#include "../David Engine/daLifeBarScript.h"

namespace da
{
	using namespace enums;

	MeshRenderer::MeshRenderer()
		: Component(eComponentType::Renderer)
		, mMesh(nullptr)
		, mMaterial(nullptr)
		, mReverse(false)
	{
	}
	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();
		
		mMesh->BindBuffer();
		mMaterial->Binds();
		
		TimeConstants* timeConstants = GetOwner()->GetComponent<TimeConstants>();
		if (timeConstants)
			timeConstants->BindConstantBuffer();

		LifeBarScript* lifebar = GetOwner()->GetComponent<LifeBarScript>();
		if (lifebar)
			lifebar->BindConstantBuffer();

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
			animator->Binds();

		BindReverseCB();
		
		
		mMesh->Render();
		mMaterial->Clear();
	}
	void MeshRenderer::BindReverseCB()
	{
		renderer::ReverseCB cbData = {};
		cbData.Reverse = mReverse;

		graphics::ConstantBuffer* reverseCB = renderer::constantBuffer[(UINT)graphics::eCBType::Reverse];
		reverseCB->SetData(&cbData);
		reverseCB->Bind(graphics::eShaderStage::VS);
	}
}
