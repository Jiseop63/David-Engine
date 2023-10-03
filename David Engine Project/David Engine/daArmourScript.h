#pragma once
#include "daScript.h"
#include "daMeshRenderer.h"
#include "daItemIconScript.h"
#include "daItemSlotScript.h"
namespace da
{
	class ArmourScript : public Script
	{
	public:
		ArmourScript();
		virtual ~ArmourScript();

		virtual void Initialize() override;
		virtual void Update() override;


	public:
		void AddIconScript(GameObject* iconObject);
		void SetBackup(bool isBackupArmour) { mBackup = isBackupArmour; }
		void SetSlotScript(ItemSlotScript* slotScript) { mSlotScript = slotScript; }
	public:
		void ChangeArmour();
		void ChangeIcon();
		void ClearPosition() { mIconScript->SetIconPosition(mPanelTransform->GetPosition()); }

	protected:
		Transform*		mPanelTransform;
		MeshRenderer*	mPanelRenderer;
		
	protected:
		ItemSlotScript* mSlotScript;
		ItemIconScript* mIconScript;

	private:
		float			mPadding;
		float			mSwapTime;
		bool			mBackup;
	};
}