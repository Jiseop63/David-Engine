#pragma once
#include "daPortalScript.h"

namespace da
{
	enum class PortalState
	{
		Idle,
		Open,
		Close,
	};

	class PortalWallScript;
	class DungeonPortalScript : public PortalScript
	{
	public:
		DungeonPortalScript();
		virtual ~DungeonPortalScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void OnCollisionEnter(Collider2D* other) override;

	public:
		void SetRotate(float angle);
		void SetPosition(math::Vector3 position);
		void AddWallScript(GameObject* object);

	public:
		virtual void GatePass() override;
		virtual void GateOpen() override;
		virtual void GateClose() override;

	public:
		void IdleAnimation();
		void PortalActive();

	private:
		PortalWallScript* mWallScript;
		bool mIsClear;
	};
}