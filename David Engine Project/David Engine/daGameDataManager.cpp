#include "daGameDataManager.h"

#include "daSceneManager.h"
#include "daInventoryScript.h"
#include "daLifeBarScript.h"
#include "daDashCountScript.h"
#include "daDungeonPortalScript.h"

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
		// ���� ���� Clear�������� Ȯ��
		bool isClear;
		if (0 == mMonsterCounts[(UINT)scene])
			isClear = true;
		else
			isClear = false;

		// ��Ż �����ϱ�
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
			}
		}

		// �ܺο��� �����ϱ� ���ϵ��� current ������ �� �ű��
		mCurrentMonsterCount = mMonsterCounts[(UINT)scene];
	}

	void GameDataManager::ExitMonsterCount(eDungeonScene scene)
	{
		// ������ ���¸� �������ֱ�
		mMonsterCounts[(UINT)scene] = mCurrentMonsterCount;
	}

	void GameDataManager::DecreaseMonsterCount(std::vector<PortalScript*> portals)
	{
		// count ����
		if (0 !=mCurrentMonsterCount)
			mCurrentMonsterCount--;

		// ���� ���͸� ������ ���
		if (0 == mCurrentMonsterCount)
		{
			for (PortalScript* target : portals)
			{
				target->GateOpen();
			}
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