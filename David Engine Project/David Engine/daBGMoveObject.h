#pragma once
#include "daBackGroundObject.h"

namespace da
{
	class BGMoveObject : public BackGroundObject
	{
	public:
		BGMoveObject();
		virtual ~BGMoveObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	};
}