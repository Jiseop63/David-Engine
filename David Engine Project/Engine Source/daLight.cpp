#include "daLight.h"
#include "daRenderer.h"
#include "daTransform.h"
#include "daGameObject.h"

namespace da
{
	using namespace graphics;

	Light::Light()
		: Component(enums::eComponentType::Light)
		, mAttribute{}
	{
	}

	Light::~Light()
	{
	}

	void Light::LateUpdate()
	{
		renderer::lights.push_back(this);

		Transform* transform = GetOwner()->GetTransform();
		math::Vector3 myPosition = transform->GetPosition();
		mAttribute.Position = math::Vector4(myPosition.x, myPosition.y, myPosition.z, 1.0f);
		mAttribute.Direction = math::Vector4(transform->Forward().x, transform->Forward().y, transform->Forward().z, 1.0f);

	}

}
