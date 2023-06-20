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
		renderer::mesh->BindBuffer();
		renderer::shader->Binds();
		graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}
