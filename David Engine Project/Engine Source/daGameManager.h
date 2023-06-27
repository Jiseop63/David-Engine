#pragma once

namespace da
{
	class GameManager
	{
	public:
		static bool IsOverrayOn() { return mIsoverlayOn; }
		static void TurnOverlay(bool value);

		static void DisappearObjects();
		static void TurnOnOverlay();

	private:
		static bool mIsoverlayOn;
	};
}