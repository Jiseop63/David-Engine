#pragma once

namespace da
{
	class GameManager
	{
	public:
		static bool IsOverrayOn() { return mIsoverlayOn; }
		static void TurnOverlay(bool value) { mIsoverlayOn = value; }



	private:
		static bool mIsoverlayOn;
	};
}