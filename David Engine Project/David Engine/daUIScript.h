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
		// �� ��ũ�� ��ǥ ã�� �Լ�
		math::Vector3 GetScreenPosision();
		// ���콺 ��ġ ã�� �Լ�
		// �浹 Ȯ���ϴ� �Լ�

	protected:
		void changeTexture(std::shared_ptr<graphics::Texture> texture);

	protected:
		std::shared_ptr<graphics::Texture> mFirstTexture;
		std::shared_ptr<graphics::Texture> mSecondTexture;
	};
}
