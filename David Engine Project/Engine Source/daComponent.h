#pragma once
#include "daEntity.h"

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
		void ApplyComponentUsing(bool value = true) { mUsing = value; }
		bool IsComponentUsing() { return mUsing; }

	protected:
		bool						mUsing;
	private:
		const enums::eComponentType mComponentType;
		GameObject*					mOwner;
	};
}