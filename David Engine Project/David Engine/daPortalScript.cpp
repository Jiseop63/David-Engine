#include "daPortalScript.h"
#include "daGameObject.h"
#include "daSceneManager.h"
#include "daPlayerScript.h"
namespace da
{
	PortalScript::PortalScript()
		: mPortalTransform(nullptr)
		, mPortalRenderer(nullptr)
		, mPortalAnimator(nullptr)
		, mPortalBodyCollider(nullptr)
		, mExitPosition(math::Vector3::Zero)
		, mGatePass(false)
		, mGateClose(false)
		, mGateOpen(false)
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
		PlayerScript* player = SceneManager::GetPlayerScript();
		player->ClearCreature();
		player->SetCreaturePosition(mExitPosition);
		SceneManager::LoadScene(mSceneName);
	}
	void PortalScript::GatePass()
	{
		mGatePass = true;
	}
	void PortalScript::GateClose()
	{
		mGateClose = true;
	}
	void PortalScript::GateOpen()
	{
		mGateOpen = true;
	}
}
