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

	public:
		void SetSceneName(const std::wstring sceneName) { mSceneName = sceneName; }

	protected:
		Transform*		mPortalTransform;
		MeshRenderer*	mPortalRenderer;
		Animator*		mPortalAnimator;
		Collider2D*		mPortalBodyCollider;

	protected:
		std::wstring	mSceneName;
	};
}