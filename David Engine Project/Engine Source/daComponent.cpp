#include "daComponent.h"

namespace da
{
	using namespace enums;

	Component::Component(eComponentType type)
		: mColliderType(type)
		, mOwner(nullptr)
	{
	}
	Component::~Component()
	{
	}
	void Component::Initialize()
	{
	}
	void Component::Update()
	{
	}
	void Component::LateUpdate()
	{
	}
	void Component::Render()
	{
	}
}