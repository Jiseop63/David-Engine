#pragma once
#include "daEntity.h"
#include "daComponent.h"
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
		template <typename T>
		T* GetComponent()
		{
			T* component;
			for (T* comp : mComponents)
			{
				component = dynamic_cast<T*>(comp);
				if (component != nullptr)
					return component;
			}

			return nullptr;
		}
		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();

			Component* buff
				= dynamic_cast<Component*>(comp);

			if (buff == nullptr)
				return nullptr;

			mComponents.push_back(buff);

			return comp;
		}

	private:
		eObjectState mState;
		std::vector<Component*> mComponents;
	};
}