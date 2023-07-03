#pragma once
#include "daScript.h"
#include "daTexture.h"

namespace da
{
	class UIScript : public Script
	{
	public:
		UIScript();
		virtual ~UIScript();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

	public:
		void SetUITextures(std::shared_ptr<graphics::Texture> first, std::shared_ptr<graphics::Texture> second);

	protected:
		// 내 스크린 좌표 찾는 함수
		math::Vector3 GetScreenPosision();
		// 마우스 위치 찾는 함수
		// 충돌 확인하는 함수

	protected:
		void changeTexture(std::shared_ptr<graphics::Texture> texture);

	protected:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;
	};
}
