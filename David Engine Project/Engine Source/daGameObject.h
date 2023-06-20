#pragma once
#include "daEntity.h"
#include "daComponent.h"
namespace da
{
	class GameObject : public Entity
	{
	public:
		enum eObjectState
		{
			Active,
			Paused,
			Dead,
		};
	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	private:
		eObjectState mState;
	};
}