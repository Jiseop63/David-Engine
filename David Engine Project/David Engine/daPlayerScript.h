#pragma once
#include "daScript.h"
#include "daRigidbody.h"
namespace da
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize();
		virtual void Update();

		void GetInput();
		void MoveFunc(math::Vector2 dir);

		// юс╫ц

		void GetDamage();
		void GetHeal();
	protected:
		Rigidbody* mRigidbody;
		
	private:
		float mMoveSpeed;
	};
}