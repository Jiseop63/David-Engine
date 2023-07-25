#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"

namespace da
{
	class ArmourScript : public Script
	{
	public:
		ArmourScript();
		virtual ~ArmourScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetBackup(bool isBackupArmour) { mBackup = isBackupArmour; }
		void ChangeArmour();
	protected:
		Transform*		mTransform;
		MeshRenderer*	mPanelRenderer;
		MeshRenderer*	mIconRenderer;

	private:
		float			mPadding;
		float			mSwapTime;
		bool			mBackup;
	};
}