#include "daGameObject.h"
#include "daRenderer.h"
namespace da
{
	GameObject::GameObject()
		: mState(eObjectState::Active)
	{
	}
	GameObject::~GameObject()
	{
	}
	void GameObject::Initialize()
	{
	}
	void GameObject::Update()
	{
	}
	void GameObject::LateUpdate()
	{
	}
	void GameObject::Render()
	{
	}
}
