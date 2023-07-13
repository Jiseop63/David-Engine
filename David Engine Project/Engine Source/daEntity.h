#pragma once
#include "David Engine.h"

// Screen Position
#define MaxPositionX 6.830f 
#define MaxPositionY 3.840f

// BG
#define BackgroundZ	0.0050f
#define BackLayerZ	0.0040f
#define FrontLayerZ 0.0030f

// Object
#define Objects		0.0f;

// UI
#define HUDZ		-0.0010f
#define OverlayZ	-0.0030f
#define CursorZ		-0.0050f

namespace da
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity();

		void SetName(const std::wstring& name) { mName = name; }
		std::wstring& GetName() { return mName; }
		void SetID(UINT64 id) { mID = id; }
		UINT64 GetID() { return mID; }

	private:
		std::wstring mName;
		UINT64 mID;
	};
}
