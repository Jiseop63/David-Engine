#pragma once
#include "daScript.h"

namespace da
{
	class ProjectileScript : public Script
	{
	public:
		ProjectileScript();
		virtual ~ProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;

		// 콜라이더 세팅 (외부에서)
		// 유지시간 & 종료 조건

	};
}