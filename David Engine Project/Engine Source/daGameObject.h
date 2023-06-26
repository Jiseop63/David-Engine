#pragma once
#include "daEntity.h"
#include "daComponent.h"
#include "daScript.h"
namespace da
{
	class GameObject : public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		enum eObjectState
		{
			Active,
			Paused,
			Dead,
		};

		// 외부에서 오브젝트 타입에 따라 전용 호출이 가능할 수 있도록 타입을 나눔
		enum eObjectType
		{
			None, // <- 아마 카메라
			Background,
			Overlay,
			HUD,
			Creature,
			Effect,
			ENV,
			End,
		};
		template <typename T>
		T* GetComponent()
		{
			T* findComponent;
			for (Component* component : mComponents)
			{
				findComponent = dynamic_cast<T*>(component);
				if (nullptr != findComponent)
					return findComponent;
			}
			for (Script* script : mScripts)
			{
				findComponent = dynamic_cast<T*>(script);
				if (nullptr != findComponent)
					return findComponent;
			}

			return nullptr;
		}

		template <typename T>
		T* AddComponent()
		{
			T* origin = new T();

			Component* component = dynamic_cast<Component*>(origin);
			Script* script = dynamic_cast<Script*>(component);

			if (nullptr == component)
				return nullptr;

			if (nullptr == script)
				mComponents.push_back(component);
			else
				mScripts.push_back(script);

			origin->SetOwner(this);

			return origin;
		}
		void SetObjectState(eObjectState state) { mObjectState = state; }
		void SetObjectType(eObjectType type) { mObjectType = type; }
	private:
		eObjectState mObjectState;
		eObjectType mObjectType;
		std::vector<Component*> mComponents;
		std::vector<Script*> mScripts;
	};
}