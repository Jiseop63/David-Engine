#pragma once
#include "daComponent.h"

namespace da
{
	class TimeConstants : public Component
	{
	public:
		TimeConstants();
		virtual ~TimeConstants();

	public:
		void SetValue(float value) { mValue = value; }

	public:
		void BindConstantBuffer();


	private:
		float mElapsedTime;
		float mValue;
	};
}