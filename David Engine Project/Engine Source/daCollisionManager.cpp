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
				if (Collider2D::eDetectionType::Inactive == left->GetDetectionType()
					|| Collider2D::eDetectionType::Inactive == right->GetDetectionType())
					return;
				//Enter
				iter->second = true;
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				return;
			}
			else
			{
				// 둘중 하나가 비활성 상태임, Exit 호출
				if (GameObject::eObjectState::Active != left->GetOwner()->GetObjectState()
					|| GameObject::eObjectState::Active != right->GetOwner()->GetObjectState())
				{					
					//Exit
					iter->second = false;
					left->OnCollisionExit(right);
					right->OnCollisionExit(left);
					return;
				}
				if (Collider2D::eDetectionType::Inactive == left->GetDetectionType()
					|| Collider2D::eDetectionType::Inactive == right->GetDetectionType())
				{
					//Exit
					iter->second = false;
					left->OnCollisionExit(right);
					right->OnCollisionExit(left);
					return;
				}

				//Stay
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
				return;
			}
		}
		else
		{
			// 충돌아님, 충돌했었음, Exit호출
			if (true == iter->second)
			{
				//Exit
				iter->second = false;
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
				return;
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
		if (left->GetColliderType() == enums::eColliderShape::Rect
			&& right->GetColliderType() == enums::eColliderShape::Circle)
		{
			retValue = RectToCircle(left, right);
			return retValue;
		}
		if (left->GetColliderType() == enums::eColliderShape::Circle
			&& right->GetColliderType() == enums::eColliderShape::Rect)
		{
			retValue = RectToCircle(right, left);
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
		
		// 임의의 로컬축 생성 (기본 형태)
		Vector3 localVertices[4] =
		{
		   Vector3{-0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, -0.5f, 0.0f}
		   ,Vector3{-0.5f, -0.5f, 0.0f}
		};

		// 비교대상인 각 충돌체의 Tr 가져오기
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		// world matrix 가져오기
		Matrix leftMatrix = leftTr->GetWorldMatrix();
		Matrix rightMatrix = rightTr->GetWorldMatrix();

		// 임시 축변수 생성
		Vector3 axes[4] = {};

		// Collider 스케일을 적용한 최종 Scale 만들기
		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMatrix;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMatrix;

		
		// RT, LB 모서리값으로 대각선 방향의 분리축 만들기

		// 로컬축에 각 OBB 객체의 Matrix(회전이 적용됨)값을 넣어서 투영 축을 만듦
		// 1번 Rect의 OBB 분리축 axes[0]와 axes[1]
		axes[0] = Vector3::Transform(localVertices[1], finalLeft);
		axes[1] = Vector3::Transform(localVertices[3], finalLeft);
		// 2번 Rect의 OBB 분리축 axes[2]와 axes[3]
		axes[2] = Vector3::Transform(localVertices[1], finalRight);
		axes[3] = Vector3::Transform(localVertices[3], finalRight);


		/*for (size_t i = 0; i < 4; i++)
		{
			axes[i] -= Vector3::Transform(localVertices[0], finalLeft);
			axes[i] -= Vector3::Transform(localVertices[0], finalRight);
		}*/

		// 1번 OBB 분리축의 LT 모서리 값 빼기
		axes[0] -= Vector3::Transform(localVertices[0], finalLeft);
		axes[1] -= Vector3::Transform(localVertices[0], finalLeft);

		// 2번 OBB 분리축의 LT 모서리값 빼기
		axes[2] -= Vector3::Transform(localVertices[0], finalRight);
		axes[3] -= Vector3::Transform(localVertices[0], finalRight);
		// 여기까지 진행했으면, 객체의 위치정보가 제거되고, 축에 대한 투영값만 남음


		// 투영축의 z값을 0으로 밀어버림 (z값을 안쓰니까)
		for (size_t i = 0; i < 4; i++)
			axes[i].z = 0.0f;

		// 두 OBB객체의 거리값
		Vector3 centerDifference = left->GetTotalPosition() - right->GetTotalPosition();
		centerDifference.z = 0.0f;

		// 투영축을 순회하며 최소, 최대크기 비교
		for (size_t index = 0; index < 4; index++)
		{
			// 현재 순회죽인 투영축 벡터
			Vector3 Axis = axes[index];

			// 초기 충돌거리 값
			float projectionDistance = 0.0f;

			// 각 OBB의 꼭짓점 순회하기
			for (size_t edge = 0; edge < 4; edge++)
			{
				// 각 꼭짓점의 투영된 크기의 절반을 누적 // OBB의 투영된 크기의 절반
				projectionDistance += fabsf(axes[edge].Dot(Axis) / 2.0f);
			}
			// 현재 투영축에 대한 충돌 검사 // 두 객체의 거리값보다 투영된 크기가 더 작다면, 두 OBB는 충돌한게 아님
			if (projectionDistance < fabsf(centerDifference.Dot(Axis)))
				return false;
		}

		return true;
	}

	bool CollisionManager::CircleToCircle(Collider2D* left, Collider2D* right)
	{
		// 각 충돌체 위치 구하기
		Transform* leftTr = left->GetOwner()->GetTransform();
		Vector2 leftPosition = leftTr->GetPosition() + left->GetCenter();
		Vector2 leftSize = leftTr->GetScale() * left->GetSize();
		

		Transform* rightTr = right->GetOwner()->GetTransform();
		Vector2 rightPosition = rightTr->GetPosition() + right->GetCenter();
		Vector2 rightSize = rightTr->GetScale() * right->GetSize();
		
		float positionDistanceSqr = Vector2::DistanceSquared(leftPosition, rightPosition);
		float radiusDistanceSqr = (leftSize.x + rightSize.x) * 0.5f;
		radiusDistanceSqr *= radiusDistanceSqr;

		if (radiusDistanceSqr < positionDistanceSqr)
			return false;
		return true;
	}
	bool CollisionManager::RectToCircle(Collider2D* rect, Collider2D* circle)
	{
		Transform* rectTr = rect->GetOwner()->GetTransform();
		Vector2 rectPosition = rectTr->GetPosition() + rect->GetCenter();
		Vector2 rectSize = rectTr->GetScale() * rect->GetSize();

		Transform* circleTr = circle->GetOwner()->GetTransform();
		Vector2 circlePosition = circleTr->GetPosition() + circle->GetCenter();
		Vector2 circleSize = circleTr->GetScale() * circle->GetSize();

		Vector2 betweenDistance(rectPosition.x - circlePosition.x, rectPosition.y - circlePosition.y);
		Vector2 axes[3];

		axes[0] = Vector2(1.0f, 0.0f); // 가로 축
		axes[1] = Vector2(0.0f, 1.0f); // 세로 축
		axes[2] = betweenDistance; // Rect의 중심에서 Circle의 중심까지의 방향 벡터

		// 각 축에 대한 투영을 계산하여 충돌을 검출
		for (int i = 0; i < 3; i++) {
			// 축 정규화			
			Vector2 axis = axes[i];
			axis.Normalize();

			// Circle과 Rect의 중심 사이의 거리 투영
			float rectProjection = axis.Dot(betweenDistance);
			//float circleProjection = XMVector2Dot(XMLoadFloat2(&circleToRect), axis).m128_f32[0];

			// Rect의 반대 방향으로 투영
			float circleProjection = rectProjection > 0 ? 
				-(rectSize.x / 2.0f) * axis.x - (rectSize.y / 2.0f) * axis.y 
				: (rectSize.x / 2.0f) * axis.x + (rectSize.y / 2.0f) * axis.y;

			// 축에 대한 투영 간의 거리가 Circle의 반지름보다 크면 충돌이 없음
			if (circleProjection > rectProjection + circle->GetSize().x)
				return false;
		}
		// 모든 축에 대한 투영 간의 거리가 Circle의 반지름보다 작으면 충돌이 있음
		return true;
	}
}
