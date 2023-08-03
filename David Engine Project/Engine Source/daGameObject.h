#pragma once
#include "daEntity.h"
#include "daComponent.h"
#include "daScript.h"
#include "daTransform.h"

namespace da
{
	class GameObject : public Entity
	{
	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize() {}
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:

		// 언젠가 써먹어야 하는 기능, 아직 확립안된 기준
		enum class eObjectState
		{
			Active,		// 씬에서 동작함
			Paused,		// Update X
			Hide,		// LateUpdate, Render X
			Inactive,	// Update, LateUpdate, Render X
			Destroy,
		};

		// 외부에서 오브젝트 타입에 따라 전용 호출이 가능할 수 있도록 타입을 나눔
		// 호출하는건 씬에서 해줘야할듯
		
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
		const std::vector<T*> GetComponents()
		{
			std::vector<T*> retComponents;

			T* findComponent;
			for (Component* targetComponent : mComponents)
			{
				findComponent = dynamic_cast<T*>(targetComponent);
				if (nullptr != findComponent)
					retComponents.push_back(findComponent);
			}
			return retComponents;			
		}
		const std::vector<Script*>& GetScripts() { return mScripts; }
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
			origin->Initialize();
			return origin;
		}

		void SetFootCollider(Collider2D* collider) { mFootCollider = collider; }
		Collider2D* GetFootCollider() { return mFootCollider; }

	public:
		void SetObjectState(eObjectState state) { mObjectState = state; }
		eObjectState GetObjectState() { return mObjectState; }
		void SetParent(GameObject* parent) { mTransform->SetParent( parent->GetTransform() ); }
		Transform* GetTransform() { return mTransform; }
		void SetLayerType(enums::eLayerType layer) { mLayerType = layer; }
		enums::eLayerType GetLayerType() { return mLayerType; }
		void SetCommonObject(bool isCommonObject) { mIsCommonObject = isCommonObject; }
		bool IsCommonObject() { return mIsCommonObject; }

		// 상속 구조..?
	public:
		void AddChildObject(GameObject* other) { mChildren.push_back(other); }
		void SetObjectStates(eObjectState state);

	protected:
		Transform*				mTransform;
		Collider2D*				mFootCollider;

	private:
		eObjectState			mObjectState;
		std::vector<Component*> mComponents;
		std::vector<Script*>	mScripts;
		std::vector<GameObject*>mChildren;
	private:
		enums::eLayerType		mLayerType;
		bool					mIsCommonObject;
	};
}