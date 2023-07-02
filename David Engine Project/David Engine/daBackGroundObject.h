#pragma once
#include "daGameObject.h"
#include "daBackGroundScript.h"
namespace da
{
	class Transform;
	class BackGroundObject : public GameObject
	{
	public:
		BackGroundObject();
		virtual ~BackGroundObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void SetPosition(math::Vector3 position) { mTransform->SetPosition(position); }
		math::Vector3 GetPosition() { return mTransform->GetPosition(); }
		void SetScale(math::Vector3 scale) { mTransform->SetScale(scale); }
		math::Vector3 GetScale() { return mTransform->GetScale(); }

	};
}

/*
*  지금으로선 딱히 하는 일은 없음
*/