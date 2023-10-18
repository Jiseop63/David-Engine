#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daRigidbody.h"

namespace da
{
	class ItemBoxScript : public Script
	{
	public:
		ItemBoxScript();
		virtual ~ItemBoxScript();

		virtual void Initialize();
		virtual void Update();

	public:
		void AddIconScript(GameObject* object);

		void SetPosition(math::Vector3 position);
		void SetPosition(float x, float y) { mBoxTransform->SetPosition(math::Vector3(x, y, 0.0f)); }
		void SetItemName(const std::wstring itemName) { mItemName = itemName; }

	private:
		void takeItem();

	public:
		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other);

	protected:
		Transform*				mBoxTransform;
		MeshRenderer*			mBoxRenderer;
		Collider2D*				mBoxCollider;
		class InputIconScript*	mInputIcon;
		bool					mFocused;
		std::wstring			mItemName;
	};
}