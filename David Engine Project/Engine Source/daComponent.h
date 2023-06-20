#pragma once
#include "daEntity.h"

namespace da
{
	using namespace enums;
	class Component : public Entity
	{
	public:
		Component();
		virtual ~Component();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	private:
		const eComponentType mType;
	};
}