#include "daCursorScript.h"

#include "daInput.h"

#include "daGameObject.h"
#include "daTransform.h"

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
}
