#include "daBansheeScript.h"
#include "daSceneManager.h"

#include "daGameDataManager.h"
#include "daTime.h"
#include "daInput.h"

#include "daResources.h"
#include "daGameObject.h"

#include "daBansheeCombatScript.h"
#include "daPlayerScript.h"

#include "daBansheeProjectileScript.h"

#include "daDungeonPortalScript.h"


namespace da
{
	BansheeScript::BansheeScript()
		: mMonsterActiveState(eBansheeState::Idle)
		, mChaseDurationTime(0.0f)
		, mChaseDurationDecay(0.0f)
		, mDistanceFromPlayer(0.0f)

		, mPrepareAttack(false)
		, mPrepareDurationTime(0.0f)
		, mPrepareDurationDecay(0.0f)

		, mAttackProgress(false)
		, mReadyDurationTime(0.0f)
		, mReadyDurationDecay(0.0f)
		, mIsAttacked(false)
	{
	}
	BansheeScript::~BansheeScript()
	{
	}
	void BansheeScript::Initialize()
	{
		MonsterScript::Initialize();
		// ������Ʈ ��������
		GetOwner()->SetName(L"BansheeObject");
		SetName(L"BansheeScript");

		mCreatureRigidbody->ApplyComponentUsing(false);
		mCreatureFootCollider->ApplyComponentUsing(false);


		// ���� �ʱ�ȭ
		mCreatureStat.MaxHP = 22.0f;
		mCreatureStat.CurHP = 22.0f;

		mMonsterAttackStat.AttackRange = 4.0f;
		mMonsterAttackStat.AttackDelayAccumulateTime = 0.0f;
		mMonsterAttackStat.AttackDelay = 4.0f;

		//mCreatureStat.MoveSpeed = 1.50f;

		mChaseDurationTime = 1.50f;
		mChaseDurationDecay = mChaseDurationTime;
		mPrepareDurationTime = 1.750f;
		mPrepareDurationDecay = mPrepareDurationTime;
		mReadyDurationTime = 7.50f;
		mReadyDurationDecay = mReadyDurationTime;

		// �浹ü �����ϱ�
		// body & foot
		{
			mCreatureBodyCollider->SetName(L"BansheeBodyCollider");
			mCreatureBodyCollider->SetSize(math::Vector2(0.30f, 0.40f));
			mCreatureBodyCollider->SetCenter(math::Vector2(0.0f, -0.10f));			
		}
		// senser
		{
			mMonsterSensorCollider->SetName(L"BansheeSensorCollider");
			mMonsterSensorCollider->SetDetectionType(Collider2D::eDetectionType::Sensor);
			//mMonsterSensorCollider->SetColliderShapeType(enums::eColliderShape::Circle);
			mMonsterSensorCollider->SetSize(math::Vector2(mMonsterAttackStat.AttackRange, mMonsterAttackStat.AttackRange));
		}

		// �ִϸ��̼� �����ϱ�

		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"BansheeSpriteSheet");
		mCreatureAnimator->Create(L"BansheeIdle", texture, math::Vector2(0.0f, 0.0f), math::Vector2(20.0f, 22.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f, 60.0f);
		mCreatureAnimator->Create(L"BansheeAttack", texture, math::Vector2(0.0f, 22.0f), math::Vector2(20.0f, 22.0f), 6, math::Vector2(0.0f, 0.0f), 0.1f, 60.0f);
		mCreatureAnimator->Create(L"BansheeDead", texture, math::Vector2(0.0f, 22.0f), math::Vector2(20.0f, 22.0f), 1, math::Vector2(0.0f, 0.0f), 0.1f, 60.0f);
		mCreatureAnimator->CompleteEvent(L"BansheeAttack") = std::bind(&BansheeScript::retIdle, this);

		mCreatureAnimator->PlayAnimation(L"BansheeIdle");

		mPlayerScript = SceneManager::GetPlayerScript();
	}
	void BansheeScript::Update()
	{
		if (!mIsControl)
			return;
		monsterFSM();
		lifeCheck();
	}
	void BansheeScript::LateUpdate()
	{
		CreatureScript::visualUpdate();
	}
	void BansheeScript::AddActionUnit(GameObject* unit)
	{
		BansheeProjectileScript* projectile = unit->AddComponent<BansheeProjectileScript>();
		projectile->SetOwnerScript(this);
		mActionUnits.push_back(projectile);
	}
	CombatScript* BansheeScript::AddCombatObject(GameObject* object)
	{
		BansheeCombatScript* combatScript = object->AddComponent<BansheeCombatScript>();
		combatScript->SetOwnerScript(this);
		mMonsterCombatScript = combatScript;

		return mMonsterCombatScript;
	}
	void BansheeScript::retIdle()
	{
		mCreatureAnimator->PlayAnimation(L"BansheeIdle");
	}
	void BansheeScript::calcTargetDir()
	{
		math::Vector3 bansheePosition = mCreatureTransform->GetPosition();
		math::Vector3 playerPosition = mPlayerScript->GetOwner()->GetTransform()->GetPosition();
		mDistanceFromPlayer = playerPosition.x - mCreatureTransform->GetPosition().x;
		mCreatureDir = math::Vector2(playerPosition.x - bansheePosition.x, playerPosition.y - bansheePosition.y);
	}
	void BansheeScript::lifeCheck()
	{
		if (0 >= mCreatureStat.CurHP)
			ChangeState(eBansheeState::Dead);
	}
	void BansheeScript::ChangeState(eBansheeState state)
	{
		mMonsterActiveState = state;		
	}
	void BansheeScript::monsterFSM()
	{
		switch (mMonsterActiveState)
		{
		case da::eBansheeState::Idle:
			BansheeHandleIdle();
			break;
		case da::eBansheeState::Attack:
			BansheeHandleAttack();
			break;
		case da::eBansheeState::Dead:
			BansheeHandleDead();
			break;
		default:
			break;
		}
	}
	void BansheeScript::BansheeHandleIdle()
	{
		if (mDetectPlayer && !mIsAttacked)
		{
			ChangeState(eBansheeState::Attack);
			mIsAttacked = true;
		}
	}
	void BansheeScript::BansheeHandleAttack()
	{
		// ���� �غ� ������
		prepareForAttack();
		// ���� ����
		doAttack();
		// �ĵ�����
		readyForAttackDelay();
	}
	void BansheeScript::BansheeHandleDead()
	{
		if (!mIsDead)
		{
			// ��� ����Ʈ ����
			MonsterScript::MonsterDeadEffects();

			GameDataManager::DecreaseMonsterCount(SceneManager::GetActiveScene()->GetPortals());

			mCreatureRigidbody->ApplyComponentUsing(true);
			mMonsterSensorCollider->ApplyComponentUsing(false);
			mCreatureBodyCollider->ApplyComponentUsing(false);
			mCreatureFootCollider->ApplyComponentUsing(true);
			mCreatureFootCollider->SetSize(math::Vector2(0.10f, 0.20F));
			mCreatureFootCollider->SetCenter(math::Vector2(0.0f, -0.20F));
			mCreatureAnimator->PlayAnimation(L"BansheeDead");
		}
	}
	void BansheeScript::findingPlayer()
	{
		// �÷��̾ �������� ���� �ִٸ� or ���ٸ�
		if (mDetectPlayer)
			mChaseDurationDecay = mChaseDurationTime;
		else
		{
			mChaseDurationDecay -= (float)Time::DeltaTime();
			if (0.0f >= mChaseDurationDecay)
			{
				mChaseDurationDecay = mChaseDurationTime;
				ChangeState(eBansheeState::Idle);
			}
		}
	}
	void BansheeScript::prepareForAttack()
	{
		// ������ �����ٸ� �ѱ��
		if (mPrepareAttack)
			return;

		// ���� ����
		mPrepareDurationDecay -= (float)Time::DeltaTime();
		if (0 >= mPrepareDurationDecay)
		{
			mPrepareDurationDecay = mPrepareDurationTime;	// ���� �ð��� �ʱ�ȭ
			mPrepareAttack = true;							// ����, �ĵ� ���� Ȱ��ȭ
		}
	}
	void BansheeScript::doAttack()
	{
		// ���� �������̰� �������� �ѱ��
		if (!mPrepareAttack)
			return;

		// �����ϱ� ���̶��
		if (!mAttackProgress)
		{
			//mMonsterCombatScript->ToDoAttack();									// ���� ��� ȣ��
			for (int index = 0; index < 10; ++index)
			{
				ActionUnitScript* actionUnit = CreatureScript::callActionUnit();

				structs::sActionUnitTypes mMonsterUnitTypes = {};
				mMonsterUnitTypes.Usage = enums::eUnitUsageType::AnimationProjectile;
				mMonsterUnitTypes.LifeCycle = enums::eUnitLifeType::Range;
				mMonsterUnitTypes.Action = enums::eUnitActionType::UsingDirection; 
				actionUnit->SetUnitTypes(mMonsterUnitTypes);

				structs::sActionUnitStat mMonsterUnitInfo = {};
				mMonsterUnitInfo.Speed = 2.0f;
				mMonsterUnitInfo.Range = 4.50f;
				mMonsterUnitInfo.Animation.Idle = L"BansheeBulletIdle";
				mMonsterUnitInfo.Animation.Action = L"BansheeBulletHit";
				mMonsterUnitInfo.AtaackDamage = 1.50f;
				actionUnit->SetUnitInfo(mMonsterUnitInfo);
				actionUnit->SetUnitScale(math::Vector3(2.250f, 2.250f, 1.0f));

				math::Vector3 moveDirection = math::daRotateVector3(math::Vector3::UnitY, index * 0.620f);
				actionUnit->SetUnitDirection(moveDirection);

				actionUnit->SetUnitOffset(math::Vector3(0.0f, -0.20f, 0.0f));
				actionUnit->OnActive();
			}
			mCreatureAnimator->PlayAnimation(L"BansheeAttack", false);	// �ִϸ��̼� ȣ��
			mAttackProgress = true;													// ���� �������� �ѱ��
			mIsAttacked = false;													// �ִϸ��̼� �ʱ�ȭ
			mCreatureAudio->Play(Resources::Find<AudioClip>(L"LilithAttack"), 60.0f);
		}
	}
	void BansheeScript::readyForAttackDelay()
	{
		if (!mPrepareAttack)
			return;

		if (mAttackProgress)
		{
			mReadyDurationDecay -= (float)Time::DeltaTime();
			if (0 >= mReadyDurationDecay)
			{
				mReadyDurationDecay = mReadyDurationTime;	// �ĵ� �ð��� �ʱ�ȭ
				mPrepareAttack = false;						// ���� ���� �ʱ�ȭ
				mAttackProgress = false;					// ���� �ִϸ��̼� ȣ�� ���� �ʱ�ȭ
				ChangeState(eBansheeState::Idle);			// ���� ����
			}
		}
	}
}