#pragma once
#include "daEntity.h"
#include "daGameObject.h"

namespace da
{
	class GameObject;
	class Component : public Entity
	{
	public:
		Component(enums::eComponentType type);
		virtual ~Component();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	public:
		GameObject* GetOwner() { return mOwner; }
		void SetOwner(GameObject* target) { mOwner = target; }

	private:
		const enums::eComponentType mType;
		GameObject* mOwner;
	};
}