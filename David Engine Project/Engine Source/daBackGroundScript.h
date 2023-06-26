#pragma once
#include "daScript.h"
#include "daTransform.h"

namespace da
{
	class Transform;
	class BackGroundScript : public Script
	{
	public:
		BackGroundScript();
		virtual ~BackGroundScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();


	public:
		void SetPosition(math::Vector3 position) { mTransform->SetPosition(position); }
		math::Vector3 GetPosition() { return mTransform->GetPosition(); }
		void SetScale(math::Vector3 scale) { mTransform->SetScale(scale); }
		math::Vector3 GetScale() { return mTransform->GetScale(); }


	public:
		void TurnOverlayDisappear(bool value) { mIsoverlayOn = value; }

		void TurnMovingObject(bool value) { mIsMovingObj = value; }
		void SetMoveValue(float value) { mMoveValue = value; }


	private:
		void movePeriodically();

	private:
		Transform* mTransform;
		bool mIsMovingObj;
		bool mIsoverlayOn;
		float mMoveValue;
		float mTurnSpeed;
		float mMoveSpeed;


	};
}
