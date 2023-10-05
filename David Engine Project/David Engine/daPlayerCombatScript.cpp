#include "daPlayerCombatScript.h"

#include "daGameDataManager.h"
#include "daSceneManager.h"
#include "daInput.h"

#include "daGameObject.h"
#include "daResources.h"
#include "daTime.h"

#include "daActionUnitScript.h"
#include "daPlayerScript.h"

namespace da
{
	PlayerCombatScript::PlayerCombatScript()
		: mWeaponItem(nullptr)
		, mWeaponTexture(nullptr)
		, mWeaponCooldownReady(true)
		, mWeaponCooldownTime{}
	{
	}
	PlayerCombatScript::~PlayerCombatScript()
	{
	}

	void PlayerCombatScript::Initialize()
	{
		CombatScript::Initialize();
	}
	void PlayerCombatScript::Update()
	{
		if (Input::GetKeyDown(eKeyCode::LBTN))
		{
			ToDoAttack();
		}		
		updateAttackCooldown();
	}
	void PlayerCombatScript::LateUpdate()
	{
		CombatScript::updateWeaponPosition();
		CombatScript::updateReverseRenderer();
		updateWeaponRotate();
	}
	void PlayerCombatScript::EquipWeapon(ItemScript* weaponItem)
	{
		// �ʱ�ȭ
		if (mWeaponItem)
			mWeaponItem->GetItemAttackStat().AttackDelayTime.Clear();
		mWeaponItem = weaponItem;

		// ������ ���� ����
		if (weaponItem)
		{
			mWeaponCooldownReady = true;
			mWeaponCooldownTime = mWeaponItem->GetItemAttackStat().AttackDelayTime;
		}
		
		// ������ ������ ����
		if (!mWeaponItem)
		{
			mCombatRenderer->ChangeMaterialTexture(nullptr);
			mCombatAnimator->ApplyComponentUsing(false);
		}
		else
		{
			if (enums::eItemRenderType::Animation == mWeaponItem->GetItemRenderType())
				mCombatAnimator->ApplyComponentUsing(true);
			else if (enums::eItemRenderType::Texture == mWeaponItem->GetItemRenderType())
			{
				mCombatRenderer->ChangeMaterialTexture(mWeaponItem->GetItemTexture());
			}
			mCombatTransform->SetScale(mWeaponItem->GetItemScale());			
		}		
	}
	void PlayerCombatScript::updateWeaponRotate()
	{
		// # �� ��ġ ��������
		math::Vector3 myPosition = mCombatTransform->GetPosition();

		// ���� ȸ������ ���ϱ�
		float angle = atan2(mOwnerDir.y, mOwnerDir.x);
		mWeaponAngle = angle;

		// Weapon Position ���ϱ�
		math::Vector2 armUp(-0.050f, 0.0f);
		math::Vector2 armDown(0.050f, -0.40f);

		// �����Ҷ����� ���� ȸ������ ��ġ�� ����
		if (isLeft())
		{
			if (!mWeaponAttacked)
			{
				angle += 2.710f;
				myPosition.x += armUp.x;
				myPosition.y += armDown.y;
			}
			else
			{
				angle -= 0.3930f;
				myPosition.x += armDown.x;
			}
		}
		else
		{
			if (!mWeaponAttacked)
			{
				angle -= 2.710f;
				myPosition.x -= armUp.x;
				myPosition.y += armDown.y;

			}
			else
			{
				angle += 0.3930f;
				myPosition.x -= armDown.x;
			}
		}

		myPosition.x -= mOwnerDir.x * 0.050f;
		myPosition.y -= mOwnerDir.y * 0.050f;
		// ����� ���� Weapon Transform�� �����ϱ�
		mCombatTransform->SetRotation(math::Vector3(0.0f, 0.0f, angle));
		mCombatTransform->SetPosition(myPosition);
	}
	void PlayerCombatScript::updateAttackCooldown()
	{
		if (!mWeaponCooldownReady)
		{
			mWeaponCooldownTime.Start += (float)Time::DeltaTime();

			if (mWeaponCooldownTime.End <= mWeaponCooldownTime.Start)
			{
				mWeaponCooldownReady = true;
				mWeaponCooldownTime.Start = 0.0f;
			}
		}
	}
	//void PlayerCombatScript::EquipWeapon()
	//{
	//	// ���߿��� ���⼭ �����ϴ°� �ƴ϶� �������� ���õǾ �Ѱ��� ������
	//	switch (mWeaponInfo->WeaponName)
	//	{
	//	case enums::eWeaponName::Default:
	//	{
	//		// ���� ���� ����
	//		mWeaponInfo->IsAnimationType = false;
	//		mWeaponInfo->AttackStat.AttackDelayTime = 0.450f;
	//		mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
	//		mWeaponInfo->AttackStat.AtaackDamage = 5.0f;
	//		
	//		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
	//		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.50f;
	//		mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
	//		mProjectileSize = math::Vector2(2.50f, 1.750f);

	//		mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
	//		mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
	//		mWeaponInfo->ProjectileStat.EffectName = L"Swing";
	//		mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

	//		if (mWeaponTexture)
	//			mCombatRenderer->ChangeMaterialTexture(mWeaponTexture);

	//		GameObject::eObjectState test = GetOwner()->GetObjectState();

	//		int a = 0;
	//	}
	//	break;
	//	case enums::eWeaponName::LongSword:
	//	{
	//		// ���� ���� ����
	//		mWeaponInfo->IsAnimationType = false;
	//		mWeaponInfo->AttackStat.AttackDelayTime = 0.450f;
	//		mWeaponInfo->AttackStat.AttackDelayAccumulateTime = 0.0f;
	//		mWeaponInfo->AttackStat.AtaackDamage = 5.0f;
	//		mWeaponInfo->ProjectileStat.ProjectileValidTime = 0.250f;
	//		mWeaponInfo->ProjectileStat.ProjectileCenterPadding = 0.50f;
	//		mWeaponInfo->ProjectileStat.ProjectileRange = 3.5f;
	//		mProjectileSize = math::Vector2(2.50f, 1.750f);
	//		mCombatTransform->SetScale(math::Vector3(0.090f * 4.0f, 0.440f * 4.0f, 1.0f));
	//		mWeaponTexture = Resources::Find<Texture>(L"LongSwordTestTexture");
	//		mWeaponInfo->ProjectileStat.EffectName = L"Swing";
	//		mEffectScale = math::Vector3(2.50f, 2.50f, 1.0f);

	//		if (mWeaponTexture)
	//			mCombatRenderer->ChangeMaterialTexture(mWeaponTexture);
	//	}
	//	break;
	//	default:
	//		break;
	//	}
	//}
	
	void PlayerCombatScript::ToDoAttack()
	{
		if (!mWeaponItem)
		{
			ItemScript* activeItem = dynamic_cast<PlayerScript*>(mOwnerScript)->GetInventoryScript()->GetActiveItemScript();
			if (activeItem)
				EquipWeapon(activeItem);
			else
				return;
		}

		if (mWeaponCooldownReady)
		{
			if (enums::eItemRenderType::Animation == mWeaponItem->GetItemRenderType())
			{
				// Play animation 
				mCombatAnimator->PlayAnimation(mWeaponItem->GetItemAnimationInfo().Name, mWeaponItem->GetItemAnimationInfo().Loop);
			}
			else
			{
				// Change texture
				if (mWeaponAttacked)
					mWeaponAttacked = false;
				else
					mWeaponAttacked = true;
			}

			ActionUnitScript* actionUnit = mOwnerScript->callActionUnit();
			
			actionUnit->SetUnitTypes(mWeaponItem->GetItemUnitTypes());
			actionUnit->SetUnitInfo(mWeaponItem->GetItemUnitInfo());
			actionUnit->SetUnitAnimation(mWeaponItem->GetItemUnitAnimationInfo());
			actionUnit->SetUnitAttackStat(mWeaponItem->GetItemAttackStat());

			actionUnit->SetUnitRotateAngle(mWeaponAngle - 1.570f);
			math::Vector3 offsetPosition = mWeaponItem->GetItemUnitOffset();
			offsetPosition.x *= mOwnerDir.x;
			offsetPosition.y *= mOwnerDir.y;
			actionUnit->SetUnitOffset(offsetPosition);

			actionUnit->OnActive();
			mWeaponCooldownReady = false;
		}
	}

}