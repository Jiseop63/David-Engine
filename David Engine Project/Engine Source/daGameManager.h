#pragma once

namespace da
{
	class GameManager
	{
	public:
		static bool IsOverrayOn() { return mIsOverlayOn; }
		static void TurnOverlay(bool value) { mIsOverlayOn = value; }

		static void DisappearObjects();
		static void TurnOnOverlay();

	private:
		static bool mIsOverlayOn;
	};
}