#pragma once
#include "david Engine.h"

namespace da
{
#define LAYER_MAX (UINT)enums::eLayerType::End

	class Collider2D;
	class CollisionManager
	{
	public:
		union ColliderID
		{
			struct
			{
				UINT left;
				UINT right;
			};
			UINT64 id;
		};

		static void Update();
		static void LayerCollision(enums::eLayerType left, enums::eLayerType right);
		static void ColliderCollision(Collider2D* left, Collider2D* right);
		static bool Intersect(Collider2D* left, Collider2D* right);

		static void SetLayer(enums::eLayerType left, enums::eLayerType right, bool enable = true);
		static void Clear();

	private:
		static void RectToRect();
		static void CircleToCircle();

	private:
		static std::bitset<LAYER_MAX> mMatrix[LAYER_MAX];
		static std::map<UINT64, bool> mCollisionMap;
	};
}