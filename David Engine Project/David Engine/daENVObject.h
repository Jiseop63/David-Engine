#pragma once
#include "daGameObject.h"
namespace da
{
	class ENVObject : public GameObject
	{
	public:
		ENVObject();
		virtual ~ENVObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void SetDisappearObject(bool value) { mIsDisappearObject = value; }
		void DoDisappear() { if (true == mIsDisappearObject) SetObjectState(eObjectState::Disappear); }

	protected:
		bool mIsDisappearObject;
	};
}