#include "daOverlayObject.h"
#include "daGameManager.h"

namespace da
{
	OverlayObject::OverlayObject()
	{
	}
	OverlayObject::~OverlayObject()
	{
	}
	void OverlayObject::Initialize()
	{
		ENVObject::Initialize();
	}
	void OverlayObject::Update()
	{
		if ( false == GameManager::IsOverrayOn() )
			return;
		ENVObject::Update();
	}
	void OverlayObject::LateUpdate()
	{
		if (false == GameManager::IsOverrayOn())
			return;
		ENVObject::LateUpdate();
	}
	void OverlayObject::Render()
	{
		if (false == GameManager::IsOverrayOn())
			return;
		ENVObject::Render();
	}
}
