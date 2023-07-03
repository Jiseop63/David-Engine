#pragma once
#include "daComponent.h"

namespace da
{
	class TimeConstants : public Component
	{
	public:
		TimeConstants();
		virtual ~TimeConstants();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	public:
		void SetValue(float value) { mValue = value; }

	public:
		void BindConstantBuffer();


	private:
		float mElapsedTime;
		float mValue;
	};
}