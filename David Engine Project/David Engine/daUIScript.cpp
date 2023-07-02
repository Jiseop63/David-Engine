#include "daUIScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"

#include "daResources.h"
#include "daTexture.h"

namespace da
{
	UIScript::UIScript()
		: mFirstTexture(nullptr)
		, mSecondTexture(nullptr)
	{
	}
	UIScript::~UIScript()
	{
	}
	void UIScript::Initialize()
	{
	}
	void UIScript::Update()
	{
	}
	void UIScript::LateUpdate()
	{
	}
	void UIScript::Render()
	{
	}
	void UIScript::SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mFirstTexture = first;
		mSecondTexture = second;
		meshRenderer->ChangeTexture(first);
	}
	void UIScript::changeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
	}
}
