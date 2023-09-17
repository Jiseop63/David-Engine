#pragma once
#include "daScript.h"
	
namespace da
{
	class CombatScript;
	class ProjectileScript : public Script
	{
	public:
		ProjectileScript();
		virtual ~ProjectileScript();

		virtual void Initialize() override;
		virtual void Update() override;


	public:
		void OnActive();

	private:
		void projectileProcess();
		void meleeProcess();
		void rangeProcess();
		void bodyProcess();

		void outOfTime();

	public:
		void SetCombatScript(CombatScript* weapon) { mCombatScript = weapon; }
		CombatScript* GetCombatScript() { return mCombatScript; }
		
		void SetProjectileInfo(structs::sProjectileStat* projectileStat) { mProjectileInfo = projectileStat; }
		void SetProjectileSize(math::Vector2 size) { mProjectileCollider->SetSize(size); }
	public:
		void ClearProjectile();

	public:
		virtual void OnCollisionEnter(Collider2D* other) override;

	protected:
		Transform*					mProjectileTransform;
		Collider2D*					mProjectileCollider;
		CombatScript*				mCombatScript;
	private:
		structs::sProjectileStat*	mProjectileInfo;

		math::Vector3				mBeginPosition;
		bool						mIsCollision;
	};
}