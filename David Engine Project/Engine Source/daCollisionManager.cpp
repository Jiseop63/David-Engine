#include "daCollisionManager.h"
#include "daSceneManager.h"
#include "daScene.h"
#include "daGameObject.h"
#include "daCollider2D.h"

namespace da
{
	std::bitset<LAYER_MAX> CollisionManager::mMatrix[LAYER_MAX] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap = {};

	void CollisionManager::Update()
	{
		for (UINT column = 0; column < (UINT)enums::eLayerType::End; ++column)
		{
			for (UINT row = 0; row < (UINT)enums::eLayerType::End; ++row)
			{
				if (true == mMatrix[column][row])
				{
					LayerCollision((enums::eLayerType)column, (enums::eLayerType)row);
				}
			}
		}
	}

	void CollisionManager::LayerCollision(enums::eLayerType left, enums::eLayerType right)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		const std::vector<GameObject*>& lefts = activeScene->GetLayer(left).GetGameObjects();
		const std::vector<GameObject*>& rights = activeScene->GetLayer(right).GetGameObjects();

		for (GameObject* leftObject : lefts)
		{
			Collider2D* leftObjectCollider = leftObject->GetComponent<Collider2D>();
			if (nullptr == leftObjectCollider)
				continue;
			if (GameObject::eObjectState::Active
				!= leftObject->GetObjectState())
				continue;

			for (GameObject* rightObject : rights)
			{
				Collider2D* rightObjectCollider = rightObject->GetComponent<Collider2D>();
				if (nullptr == rightObjectCollider)
					continue;
				if (rightObject == leftObject)
					continue;
				if (GameObject::eObjectState::Active
					!= rightObject->GetObjectState())
					continue;

				ColliderCollision(leftObjectCollider, rightObjectCollider);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
	{
		ColliderID colliderID = {};
		colliderID.left = left->GetColliderID();
		colliderID.right = right->GetColliderID();

		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(colliderID.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		if ( Intersect(left, right) )
		{
			if (false == iter->second)
			{
				//Enter
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
			}
			else
			{
				//Stay
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			if (true == iter->second)
			{
				//Exit
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		if (left->GetColliderType() == enums::eColliderType::Rect
			&& right->GetColliderType() == enums::eColliderType::Rect)
		{
			// rect <-> rect
			RectToRect();
		}
		if (left->GetColliderType() == enums::eColliderType::Circle
			&& right->GetColliderType() == enums::eColliderType::Circle)
		{
			// circle <-> circle
			CircleToCircle();
		}

		return false;
	}

	void CollisionManager::SetLayer(enums::eLayerType left, enums::eLayerType right, bool enable)
	{
		UINT row = -1;
		UINT col = -1;

		UINT iLeft = (UINT)left;
		UINT iRight = (UINT)right;

		if (iLeft <= iRight)
		{
			row = iLeft;
			col = iRight;
		}
		else
		{
			row = iRight;
			col = iLeft;
		}
		mMatrix[col][row] = enable;
	}

	void CollisionManager::Clear()
	{
		mMatrix->reset();
		mCollisionMap.clear();
	}

	void CollisionManager::RectToRect()
	{
	}

	void CollisionManager::CircleToCircle()
	{
	}

}