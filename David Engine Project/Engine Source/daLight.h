#pragma once
#include "daComponent.h"
#include "daGraphics.h"

namespace da
{
	class Light : public Component
	{
	public:
		Light();
		virtual ~Light();

		virtual void LateUpdate() override;

		graphics::LightAttribute GetAttribute() { return mAttribute; }
		void SetLightType(enums::eLightType type) { mAttribute.LightType = type; }
		enums::eLightType GetLightType() { return mAttribute.LightType; }
		void SetRadius(float radius) { mAttribute.LightRadius = radius; }
		float GetRadius() { return mAttribute.LightRadius; }
		void SetAngle(float angle) { mAttribute.LightAngle = angle; }
		float GetAngle() { return mAttribute.LightAngle; }
		void SetColor(math::Vector4 color) { mAttribute.Color = color; }
		math::Vector4 GetColor() { return mAttribute.Color; }

	private:
		graphics::LightAttribute mAttribute;

	};
}