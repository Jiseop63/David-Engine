#include "daCursorScript.h"

#include "daInput.h"

#include "daGameObject.h"
#include "daTransform.h"
#include "daMeshRenderer.h"
#include "daResources.h"

namespace da
{
	CursorScript::CursorScript()
	{
	}
	CursorScript::~CursorScript()
	{
	}
	void CursorScript::LateUpdate()
	{
		// ��(cursor) Tr ��������
		Transform* cursorTransform = GetOwner()->GetComponent<Transform>();
		Vector3 myPosition = cursorTransform->GetPosition();

		// ���콺 ��ġ ��������
		Vector3 uiMousePos = Input::GetMouseUIPosition();

		// ����ġ�� ���콺 ��ġ�� �ű��
		cursorTransform->SetPosition(uiMousePos);
	}
	void CursorScript::IsIngame(bool value)
	{
		std::shared_ptr<Texture> texture;
		if (value)
			texture = Resources::Find<Texture>(L"ShootingCursorTexture");
		else
			texture = Resources::Find<Texture>(L"BasicCursorTexture");
		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();
		mr->ChangeSlotTexture(texture);
	}
}
