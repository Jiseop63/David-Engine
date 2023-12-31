#include "daGameDataManager.h"

#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"
#include "daDungeonPortalScript.h"
#include "daBossPortalScript.h"

#include "daFairyScript.h"
#include "daItemBoxScript.h"

namespace da
{
	using namespace math;

	da::math::Vector2 GameDataManager::mCameraMovableRange = Vector2::Zero;
	da::math::Vector2 GameDataManager::mCameraMovePosition = Vector2::Zero;	

	bool GameDataManager::mDebuging = false;

	std::vector<int> GameDataManager::mMonsterCounts((UINT)eDungeonScene::End, 0);
	int GameDataManager::mCurrentMonsterCount = 0;

	void GameDataManager::EnterMonsterCount(eDungeonScene scene, std::vector<PortalScript*> portals)
	{
		if (0 == portals.size())
		{
			mCurrentMonsterCount = 0;
			return;
		}
		// 현재 씬이 Clear상태인지 확인
		bool isClear;
		if (0 == mMonsterCounts[(UINT)scene])
			isClear = true;
		else
			isClear = false;

		// 포탈 제어하기
		if (isClear)
		{
			for (PortalScript* target : portals)
			{
				target->GatePass();
			}
		}
		else
		{
			for (PortalScript* target : portals)
			{
				target->GateClose();
				SceneManager::GetPlayerScript()->DungeonClearSound();
			}
		}
		// 외부에서 접근하기 편하도록 current 변수에 값 옮기기
		mCurrentMonsterCount = mMonsterCounts[(UINT)scene];
	}

	void GameDataManager::ExitMonsterCount(eDungeonScene scene)
	{
		// 마지막 상태를 갱신해주기
		mMonsterCounts[(UINT)scene] = mCurrentMonsterCount;
	}

	void GameDataManager::DecreaseMonsterCount(std::vector<PortalScript*> portals)
	{
		// count 감소
		if (0 !=mCurrentMonsterCount)
			mCurrentMonsterCount--;

		// 만약 몬스터를 다잡은 경우
		if (0 == mCurrentMonsterCount)
		{
			bool isBossPortal = false;
			BossPortalScript* bossPortal = nullptr;
			// 던전 열어주고
			for (PortalScript* target : portals)
			{
				target->GateOpen();
				bossPortal = dynamic_cast<BossPortalScript*>(target);

				if (bossPortal)
					isBossPortal = true;
			}
			// 사운드 재생
			if (isBossPortal)
				bossPortal->PlayOpenSound();
			else
				SceneManager::GetPlayerScript()->DungeonClearSound();
			int a = 0;
			// 회복객체 배치하기
			FairyScript* fairy = SceneManager::GetActiveScene()->GetFairySript();
			ItemBoxScript* itemBox = SceneManager::GetActiveScene()->GetItemBoxScript();

			if (fairy)
				fairy->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
			if (itemBox)
				itemBox->GetOwner()->SetObjectState(GameObject::eObjectState::Active);
		}
	}

	void GameDataManager::SetCameraMovaPosition(da::math::Vector2 vector2, bool shake)
	{		
		math::Vector2 calcPos;
		if (vector2.x <= mCameraMovableRange.x
			&& vector2.x >= -mCameraMovableRange.x)
			calcPos.x = vector2.x;
		else if (vector2.x <= -mCameraMovableRange.x)
			calcPos.x = -mCameraMovableRange.x;
		else if (vector2.x >= mCameraMovableRange.x)
			calcPos.x = mCameraMovableRange.x;

		if (vector2.y <= mCameraMovableRange.y
			&& vector2.y >= -mCameraMovableRange.y)
			calcPos.y = vector2.y;
		else if (vector2.y <= -mCameraMovableRange.y)
			calcPos.y = -mCameraMovableRange.y;
		else if (vector2.y >= mCameraMovableRange.y)
			calcPos.y = mCameraMovableRange.y;
		
		if (shake)
			mCameraMovePosition = vector2;
		else
			mCameraMovePosition = calcPos;
	}
}