#pragma once
#include "daGameObject.h"

namespace da
{
	class CreatureObject : public GameObject
	{
	public:
		CreatureObject();
		virtual ~CreatureObject();
		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	
	protected:
		// void OnAttack();
		// void OnHit();
		// void OnDamage();
		
		// void OnDamaged();

	protected:
		// �ʿ��� ������Ʈ��


	private:
		// etc value..?
	};
}