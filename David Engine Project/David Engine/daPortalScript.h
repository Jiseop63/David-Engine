#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daAnimator.h"

namespace da
{
	class PortalScript : public Script
	{
	public:
		PortalScript();
		virtual ~PortalScript();

		virtual void Initialize() override;
		virtual void OnCollisionEnter(Collider2D* other) override;

	public:
		virtual void ChangeScene();
		void SetMaterial(std::shared_ptr<Material> material) { mPortalRenderer->SetMaterial(material); }
		void SetSceneName(const std::wstring sceneName) { mSceneName = sceneName; }

	public:
		virtual void GatePass();
		virtual void GateClose();
		virtual void GateOpen();



	protected:
		Transform*		mPortalTransform;
		MeshRenderer*	mPortalRenderer;
		Animator*		mPortalAnimator;
		Collider2D*		mPortalBodyCollider;

	protected:
		std::wstring	mSceneName;
		bool			mGatePass;
		bool			mGateClose;
		bool			mGateOpen;
		
	};
}