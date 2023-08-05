#include "daProjectileScript.h"
#include "daGameObject.h"

namespace da
{
	ProjectileScript::ProjectileScript()
		: mProjectileTransform(nullptr)
		, mProjectileCollider(nullptr)
		, mReqWeapon(nullptr)
	{
	}
	ProjectileScript::~ProjectileScript()
	{
	}
	void ProjectileScript::Initialize()
	{
		mProjectileTransform = GetOwner()->GetTransform();
		mProjectileCollider = GetOwner()->AddComponent<Collider2D>();
	}
	void ProjectileScript::Update()
	{
		turnOffCollision();
	}
	void ProjectileScript::turnOffCollision()
	{
		if (!mReqWeapon->IsProjectileCollision())
			GetOwner()->SetObjectState(GameObject::eObjectState::Inactive);
	}
	void ProjectileScript::SetProjectileCollider(math::Vector2 size, math::Vector2 center)
	{
		mProjectileCollider->SetSize(size);
		mProjectileCollider->SetCenter(center);
	}
}
