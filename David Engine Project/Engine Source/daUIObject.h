#pragma once
#include "daENVObject.h"

namespace da
{
	class Transform;
	class UIObject : public ENVObject
	{
	public:
		UIObject();
		virtual ~UIObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	};
}