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

		BackGroundScript* GetBGScript() { return mBackGroundScript; }

	protected:
		BackGroundScript* mBackGroundScript;
	};
}