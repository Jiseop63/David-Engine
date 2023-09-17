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
				// ���� �ϳ��� ��Ȱ�� ������, Exit ȣ��
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
			// �浹�ƴ�, �浹�߾���, Exitȣ��
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
		
		// ������ ������ ���� (�⺻ ����)
		Vector3 localVertices[4] =
		{
		   Vector3{-0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, 0.5f, 0.0f}
		   ,Vector3{0.5f, -0.5f, 0.0f}
		   ,Vector3{-0.5f, -0.5f, 0.0f}
		};

		// �񱳴���� �� �浹ü�� Tr ��������
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		// world matrix ��������
		Matrix leftMatrix = leftTr->GetWorldMatrix();
		Matrix rightMatrix = rightTr->GetWorldMatrix();

		// �ӽ� �ຯ�� ����
		Vector3 axes[4] = {};

		// Collider �������� ������ ���� Scale �����
		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMatrix;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMatrix;

		
		// RT, LB �𼭸������� �밢�� ������ �и��� �����

		// �����࿡ �� OBB ��ü�� Matrix(ȸ���� �����)���� �־ ���� ���� ����
		// 1�� Rect�� OBB �и��� axes[0]�� axes[1]
		axes[0] = Vector3::Transform(localVertices[1], finalLeft);
		axes[1] = Vector3::Transform(localVertices[3], finalLeft);
		// 2�� Rect�� OBB �и��� axes[2]�� axes[3]
		axes[2] = Vector3::Transform(localVertices[1], finalRight);
		axes[3] = Vector3::Transform(localVertices[3], finalRight);


		/*for (size_t i = 0; i < 4; i++)
		{
			axes[i] -= Vector3::Transform(localVertices[0], finalLeft);
			axes[i] -= Vector3::Transform(localVertices[0], finalRight);
		}*/

		// 1�� OBB �и����� LT �𼭸� �� ����
		axes[0] -= Vector3::Transform(localVertices[0], finalLeft);
		axes[1] -= Vector3::Transform(localVertices[0], finalLeft);

		// 2�� OBB �и����� LT �𼭸��� ����
		axes[2] -= Vector3::Transform(localVertices[0], finalRight);
		axes[3] -= Vector3::Transform(localVertices[0], finalRight);
		// ������� ����������, ��ü�� ��ġ������ ���ŵǰ�, �࿡ ���� �������� ����


		// �������� z���� 0���� �о���� (z���� �Ⱦ��ϱ�)
		for (size_t i = 0; i < 4; i++)
			axes[i].z = 0.0f;

		// �� OBB��ü�� �Ÿ���
		Vector3 centerDifference = left->GetTotalPosition() - right->GetTotalPosition();
		centerDifference.z = 0.0f;

		// �������� ��ȸ�ϸ� �ּ�, �ִ�ũ�� ��
		for (size_t index = 0; index < 4; index++)
		{
			// ���� ��ȸ���� ������ ����
			Vector3 Axis = axes[index];

			// �ʱ� �浹�Ÿ� ��
			float projectionDistance = 0.0f;

			// �� OBB�� ������ ��ȸ�ϱ�
			for (size_t edge = 0; edge < 4; edge++)
			{
				// �� �������� ������ ũ���� ������ ���� // OBB�� ������ ũ���� ����
				projectionDistance += fabsf(axes[edge].Dot(Axis) / 2.0f);
			}
			// ���� �����࿡ ���� �浹 �˻� // �� ��ü�� �Ÿ������� ������ ũ�Ⱑ �� �۴ٸ�, �� OBB�� �浹�Ѱ� �ƴ�
			if (projectionDistance < fabsf(centerDifference.Dot(Axis)))
				return false;
		}

		return true;
	}

	bool CollisionManager::CircleToCircle(Collider2D* left, Collider2D* right)
	{
		// �� �浹ü ��ġ ���ϱ�
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

		axes[0] = Vector2(1.0f, 0.0f); // ���� ��
		axes[1] = Vector2(0.0f, 1.0f); // ���� ��
		axes[2] = betweenDistance; // Rect�� �߽ɿ��� Circle�� �߽ɱ����� ���� ����

		// �� �࿡ ���� ������ ����Ͽ� �浹�� ����
		for (int i = 0; i < 3; i++) {
			// �� ����ȭ			
			Vector2 axis = axes[i];
			axis.Normalize();

			// Circle�� Rect�� �߽� ������ �Ÿ� ����
			float rectProjection = axis.Dot(betweenDistance);
			//float circleProjection = XMVector2Dot(XMLoadFloat2(&circleToRect), axis).m128_f32[0];

			// Rect�� �ݴ� �������� ����
			float circleProjection = rectProjection > 0 ? 
				-(rectSize.x / 2.0f) * axis.x - (rectSize.y / 2.0f) * axis.y 
				: (rectSize.x / 2.0f) * axis.x + (rectSize.y / 2.0f) * axis.y;

			// �࿡ ���� ���� ���� �Ÿ��� Circle�� ���������� ũ�� �浹�� ����
			if (circleProjection > rectProjection + circle->GetSize().x)
				return false;
		}
		// ��� �࿡ ���� ���� ���� �Ÿ��� Circle�� ���������� ������ �浹�� ����
		return true;
	}
}
