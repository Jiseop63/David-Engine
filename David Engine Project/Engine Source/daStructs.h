#include "daMath.h"
#include "daEnums.h"

namespace da::structs
{
	// entity
	struct sAnimationInfo
	{
		std::wstring Idle;
		std::wstring Action;
		sAnimationInfo() : Idle(), Action()
		{}
	};
	struct sActionTimeValues
	{
		float Start;
		float End;
		sActionTimeValues() : Start(0.0f), End(1.0f)
		{}
		void Clear() { Start = 0.0f; }
		bool TimeOut() { return (End <= Start) ? true : false; }
	};

	// Unit
	struct sActionUnitTypes
	{
		enums::eUnitUsageType	Usage;
		enums::eUnitLifeType	LifeCycle;
		enums::eUnitActionType	Action;
		sActionUnitTypes() : Usage(enums::eUnitUsageType::AnimationProjectile), LifeCycle(enums::eUnitLifeType::AnimationEnd), Action(enums::eUnitActionType::Stay)
		{}
	};
	struct sActionUnitStat
	{
		// range
		float				Speed;
		float				Range;
		// duration
		sActionTimeValues	DurationTime;
		// animationEnd
		sAnimationInfo		Animation;
		// using texture
		std::wstring		Texture;
		float				AtaackDamage;

		sActionUnitStat() : Speed(0.0f), Range(0.0f), DurationTime(), Animation(), Texture(), AtaackDamage(0.0f)
		{}
	};

	// Item (Weapon)
	struct sItemInfo
	{
		bool					UseAnimation;
		enums::eItemAttackType	AttackType;
		std::wstring			Sound;
		sAnimationInfo			Animation;
		sActionTimeValues		AttackDelayTime;

		sItemInfo() : UseAnimation(false), AttackType(enums::eItemAttackType::Swing), Animation(), AttackDelayTime()
		{}
	};
	

	struct sCreatureStat
	{
		float MaxHP;
		float CurHP;
		float MoveSpeed;		
				
		sCreatureStat()
			: MaxHP(0.0f), CurHP(0.0f), MoveSpeed(0.0f)
		{}
	};

	struct sMonsterAttackStat
	{
		float AttackRange;
		float AttackDelayAccumulateTime;
		float AttackDelay;
		sMonsterAttackStat()
			: AttackRange(0.0f), AttackDelayAccumulateTime(0.0f), AttackDelay(0.0f)
		{}
	};
	struct sBossProjectileStat
	{
		float ProjectileDamage;
		float ProjectileRange;
		float ProjectileSpeed;
		float ProjectileAngle;
		bool ProjectileCollision;
		sBossProjectileStat()
			: ProjectileDamage(0.0f), ProjectileRange(0.0f), ProjectileSpeed(0.0f), ProjectileAngle(0.0f), ProjectileCollision(false)
		{}
	};
	struct sJumpData
	{
		float	JumpForce;
		float	JumpAccumulateTime;
		float	JumpLimitTime;
		float	JumpForceRatio;
		bool	BufferedJump;
		bool	ExtraJump;
		sJumpData()
			: JumpForce(0.0f), JumpAccumulateTime(0.0f), JumpLimitTime(0.0f), JumpForceRatio(0.0f)
			, BufferedJump(false), ExtraJump(true)
		{}
	};
	struct sDashData
	{
		int		MaxDashCount;
		int		CurDashCount;
		float	DashForce;
		float	DashAccumulateTime;
		float	DashRegenTime;
		sDashData()
			: MaxDashCount(0), CurDashCount(0), DashForce(0.0f), DashAccumulateTime(0.0f), DashRegenTime(0.0f)
		{}
	};

}