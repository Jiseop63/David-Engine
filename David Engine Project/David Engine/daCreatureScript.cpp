#include "daCreatureScript.h"
#include "daGameObject.h"
#include "daResources.h"

namespace da
{
	using namespace math;

	CreatureScript::CreatureScript()
	{
	}
	CreatureScript::~CreatureScript()
	{
	}
	void CreatureScript::Initialize()
	{
		mRigidbody = GetOwner()->GetComponent<Rigidbody>();
		mAnimator = GetOwner()->GetComponent<Animator>();

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"SkelSprite", L"..\\Resources\\Texture\\Monster\\Skel\\SpriteSheet.png");
		mAnimator->Create(L"SkelIdle", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelMove", texture, Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f), 6, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->Create(L"SkelAttact", texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 1, Vector2(0.0f, 0.0f), 0.1f);
		mAnimator->PlayAnimation(L"SkelMove");
	}
	void CreatureScript::Update()
	{
	}
	void CreatureScript::LateUpdate()
	{
	}
	void CreatureScript::OnCollisionEnter(Collider2D* other)
	{
	}
	void CreatureScript::OnCollisionStay(Collider2D* other)
	{
	}
	void CreatureScript::OnCollisionExit(Collider2D* other)
	{
	}
}