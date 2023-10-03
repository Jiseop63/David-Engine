#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class ItemIconScript : public Script
	{
	public:
		ItemIconScript();
		virtual ~ItemIconScript();

		virtual void Initialize() override;

		void SetIconTexture(std::shared_ptr<Texture> texture) { mIconRenderer->GetMaterial()->SetTexture(texture); }
		std::shared_ptr<Texture> GetItemTexture() { return mIconRenderer->GetMaterial()->GetTexture(); }
		
		void SetIconPosition(math::Vector3 position) { mIconTransform->SetPosition(position); }

		void SetIconScale(math::Vector3 scale) { mIconTransform->SetScale(scale); }
		math::Vector3 GetIconScale() { return mIconTransform->GetScale(); }
		
	protected:
		Transform*		mIconTransform;
		MeshRenderer*	mIconRenderer;
	};
}