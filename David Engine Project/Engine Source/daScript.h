#pragma once
#include "daComponent.h"
#include "daCollider2D.h"

namespace da
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider2D* other) {}
		virtual void OnCollisionStay(Collider2D* other) {}
		virtual void OnCollisionExit(Collider2D* other) {}

		virtual void OnLandEnter(Collider2D* other) {}
		virtual void OnLandStay(Collider2D* other) {}
		virtual void OnLandExit(Collider2D* other) {}
	};
}