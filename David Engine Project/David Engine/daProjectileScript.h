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

		// �ݶ��̴� ���� (�ܺο���)
		// �����ð� & ���� ����

	};
}