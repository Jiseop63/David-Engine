#include "daCollisionManager.h"
#include "daSceneManager.h"
#include "daScene.h"
#include "daLayer.h"
#include "daGameObject.h"
#include "daCollider2D.h"

namespace da
{
	std::bitset<LAYER_MAX> CollisionManager::mCollisionMatrix[LAYER_MAX] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap = {};

	using namespace math;

	void CollisionManager::Update()
	{
		for (UINT row = 0; row < (UINT)enums::eLayerType::End; ++row)
		{
			for (UINT column = 0; column < (UINT)enums::eLayerType::End; ++column)
			{
				if (true == mCollisionMatrix[row][column])
				{
					LayerCollision((enums::eLayerType)row, (enums::eLayerType)column);
				}
			}
		}
	}

	void CollisionManager::LayerCollision(enums::eLayerType left, enums::eLayerType right)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		const std::vector<GameObject*>& lefts = activeScene->GetLayer(left).GetGameObjects();
		const std::vector<GameObject*>& rights = activeScene->GetLayer(right).GetGameObjects();

		if (enums::eLayerType::Land == left
			|| enums::eLayerType::Land == right)
			int b = 0;
		for (GameObject* leftObject : lefts)
		{
			std::vector<Collider2D*> leftObjectColliders = leftObject->GetComponents<Collider2D>();
			if (leftObjectColliders.empty())
				continue;

			for (Collider2D* leftObjectCollider : leftObjectColliders)
			{

				for (GameObject* rightObject : rights)
				{
					if (rightObject == leftObject)
						continue;

					std::vector<Collider2D*> rightObjectColliders = rightObject->GetComponents<Collider2D>();
					if (leftObjectColliders.empty())
						continue;

					for (Collider2D* rightObjectCollider : rightObjectColliders)
					{
						ColliderCollision(leftObjectCollider, rightObjectCollider);
					}
				}
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

		if (Intersect(left, right))
		{
			if (false == iter->second)
			{
				if (GameObject::eObjectState::Active != left->GetOwner()->GetObjectState()
					|| GameObject::eObjectState::Active != right->GetOwner()->GetObjectState())
					return;
				
				// 충돌함, 충돌한적 없음, Enter 호출
				if (Collider2D::eColliderDetection::Land == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Land == right->GetColliderDetection())
				{
					iter->second = true;
					left->OnGroundEnter(right);
					right->OnGroundEnter(left);
					return;
				}
				else if (Collider2D::eColliderDetection::Default == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Default == right->GetColliderDetection())
				{
					//Enter
					iter->second = true;
					left->OnCollisionEnter(right);
					right->OnCollisionEnter(left);
					return;
				}
				else
				{
					//Enter
					iter->second = true;
					left->OnWallEnter(right);
					right->OnWallEnter(left);
					return;
				}
				
			}
			else
			{
				// 둘중 하나가 비활성 상태임, Exit 호출
				if (GameObject::eObjectState::Active != left->GetOwner()->GetObjectState()
					|| GameObject::eObjectState::Active != right->GetOwner()->GetObjectState())
				{
					if (Collider2D::eColliderDetection::Land == left->GetColliderDetection()
						&& Collider2D::eColliderDetection::Land == right->GetColliderDetection())
					{
						iter->second = false;
						left->OnGroundExit(right);
						right->OnGroundExit(left);
						return;
					}
					else if (Collider2D::eColliderDetection::Default == left->GetColliderDetection()
						&& Collider2D::eColliderDetection::Default == right->GetColliderDetection())
					{
						//Exit
						iter->second = false;
						left->OnCollisionExit(right);
						right->OnCollisionExit(left);
						return;
					}
					else
					{
						//Exit
						iter->second = false;
						left->OnWallExit(right);
						right->OnWallExit(left);
						return;
					}					
				}

				// 충돌함, 충돌한적 있음, Stay 호출
				if (Collider2D::eColliderDetection::Land == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Land == right->GetColliderDetection())
				{
					left->OnGroundStay(right);
					right->OnGroundStay(left);
					return;
				}
				else if (Collider2D::eColliderDetection::Default == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Default == right->GetColliderDetection())
				{
					//Stay
					left->OnCollisionStay(right);
					right->OnCollisionStay(left);
					return;
				}
				else
				{
					left->OnWallStay(right);
					right->OnWallStay(left);
					return;
				}
			}
		}
		else
		{
			// 충돌아님, 충돌했었음, Exit호출
			if (true == iter->second)
			{
				if (Collider2D::eColliderDetection::Land == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Land == right->GetColliderDetection())
				{
					iter->second = false;
					left->OnGroundExit(right);
					right->OnGroundExit(left);
					return;
				}
				else if (Collider2D::eColliderDetection::Default == left->GetColliderDetection()
					&& Collider2D::eColliderDetection::Default == right->GetColliderDetection())
				{
					//Exit
					iter->second = false;
					left->OnCollisionExit(right);
					right->OnCollisionExit(left);
					return;
				}
				else
				{
					iter->second = false;
					left->OnWallExit(right);
					right->OnWallExit(left);
					return;
				}
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		bool retValue = false;

		if (left->GetColliderType() == enums::eColliderShape::Rect
			&& right->GetColliderType() == enums::eColliderShape::Rect)
		{
			// rect <-> rect
			retValue = RectToRect(left, right);
			return retValue;
		}

		if (left->GetColliderType() == enums::eColliderShape::Circle
			&& right->GetColliderType() == enums::eColliderShape::Circle)
		{
			// circle <-> circle
			retValue = CircleToCircle(left, right);
			return retValue;
		}

		return retValue;
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
		mCollisionMatrix[row][col] = enable;
	}

	void CollisionManager::Clear()
	{
		mCollisionMatrix->reset();
		mCollisionMap.clear();
	}

	bool CollisionManager::RectToRect(Collider2D* left, Collider2D* right)
	{
		// Rect vs Rect 
		// 0 --- 1
		// |     |
		// 3 --- 2
		Vector3 arrLocalPos[4] =
		{
		   Vector3{-0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, -0.5f, 0.0f}
		   ,Vector3{-0.5f, -0.5f, 0.0f}
		};

		// Tr 가져오기
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		// 각 world matrix 가져오기
		Matrix leftMatrix = leftTr->GetWorldMatrix();
		Matrix rightMatrix = rightTr->GetWorldMatrix();

		Vector3 Axis[4] = {};

		// Collider 스케일을 적용한 최종 Scale 만들기
		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMatrix;
		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMatrix;

		Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);
		Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);
		Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);
		Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);

		Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
		Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);

		for (size_t i = 0; i < 4; i++)
			Axis[i].z = 0.0f;
		Vector3 vc = left->GetTotalPosition() - right->GetTotalPosition();
		vc.z = 0.0f;

		Vector3 centerDir = vc;
		for (size_t i = 0; i < 4; i++)
		{
			Vector3 vA = Axis[i];

			float projDistance = 0.0f;
			for (size_t j = 0; j < 4; j++)
			{
				projDistance += fabsf(Axis[j].Dot(vA) / 2.0f);
			}

			if (projDistance < fabsf(centerDir.Dot(vA)))
				return false;
		}

		return true;
	}

	bool CollisionManager::CircleToCircle(Collider2D* left, Collider2D* right)
	{
		return false;
	}
}
