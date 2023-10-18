#pragma once
#include "daPortalScript.h"
#include "daAudioSource.h"

namespace da
{
	class BossPortalScript : public PortalScript
	{
	public:
		BossPortalScript();
		virtual ~BossPortalScript();

		virtual void Initialize() override;
		virtual void Update() override;

		virtual void OnCollisionEnter(Collider2D* other) override;
		virtual void OnCollisionExit(Collider2D* other) override;

	public:
		void SetPosition(math::Vector3 position);
		void AddIconScript(GameObject* object);

	public:
		virtual void GatePass() override;
		virtual void GateClose() override;
		virtual void GateOpen() override;

	public:
		void PlayOpenSound();
		/*void DoorOpening();
		void DoorClosing();*/

	public:
		void PlayDoorOpen();
		void PlayDoorClose();
		void HidePlayer();

	private:
		AudioSource* mPortalAudio;
		class InputIconScript* mPortalIcon;
		bool mFocused;
		bool mIsClear;
	};

}