#pragma once
#include "daUIScript.h"
#include "daTexture.h"

namespace da
{
	class ButtonScript : public UIScript
	{
	public:
		enum eButtonType
		{
			Default,
			Play,
			Exit,
		};

	public:
		ButtonScript();
		virtual ~ButtonScript();

		virtual void Update();

	public:
		void SetButtonType(eButtonType buttonType) { mButtonType = buttonType; }
		eButtonType GetButtonType() { return mButtonType; }

	protected:
		eButtonType mButtonType;
	};
}
