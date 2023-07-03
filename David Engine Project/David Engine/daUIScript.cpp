#include "daUIScript.h"

#include "daGameObject.h"
#include "daMeshRenderer.h"

#include "daResources.h"
#include "daTexture.h"

#include "daInput.h"
#include "daTransform.h"

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
		if (Input::GetKeyDown(eKeyCode::A))
		{
			GetScreenPosision();
		}
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
	math::Vector3 UIScript::GetScreenPosision()
	{
		math::Vector3 myScreenPos = GetOwner()->GetComponent<Transform>()->GetScreenPosition();
		return myScreenPos;
	}
	void UIScript::changeTexture(std::shared_ptr<graphics::Texture> texture)
	{
		MeshRenderer* meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
		meshRenderer->ChangeTexture(texture);
	}
}
