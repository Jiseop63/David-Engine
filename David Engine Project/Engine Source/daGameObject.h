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

		// ������ ��Ծ�� �ϴ� ���, ���� Ȯ���ȵ� ����
		enum class eObjectState
		{
			Active,		// ������ ������
			Paused,		// Update X
			Hide,		// LateUpdate, Render X
			Inactive,	// Update, LateUpdate, Render X
			Destroy,
		};

		// �ܺο��� ������Ʈ Ÿ�Կ� ���� ���� ȣ���� ������ �� �ֵ��� Ÿ���� ����
		// ȣ���ϴ°� ������ ������ҵ�
		
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

		// ��� ����..?
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