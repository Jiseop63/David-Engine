#include "daPortalScript.h"
#include "daGameObject.h"
#include "daSceneManager.h"

namespace da
{
	PortalScript::PortalScript()
		: mPortalTransform(nullptr)
		, mPortalRenderer(nullptr)
		, mPortalAnimator(nullptr)
		, mPortalBodyCollider(nullptr)
	{
	}
	PortalScript::~PortalScript()
	{
	}
	void PortalScript::Initialize()
	{
		mPortalTransform = GetOwner()->GetTransform();
		mPortalRenderer = GetOwner()->GetComponent<MeshRenderer>();
		mPortalAnimator = GetOwner()->AddComponent<Animator>();
		mPortalBodyCollider = GetOwner()->AddComponent<Collider2D>();
	}
	void PortalScript::OnCollisionEnter(Collider2D* other)
	{
		// 플레이어와 충돌할경우
		// fade 효과?
		ChangeScene();
	}
	void PortalScript::ChangeScene()
	{
		SceneManager::LoadScene(mSceneName);
	}
}
