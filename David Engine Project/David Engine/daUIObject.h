#pragma once
#include "daGameObject.h"

namespace da
{
	class UIObject : public GameObject
	{
	public:
		UIObject();
		virtual ~UIObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void ExcuteEvent();

	protected:


	};
}