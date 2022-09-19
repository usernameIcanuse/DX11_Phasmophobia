#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

#include "Base.h"

BEGIN(Engine)


class CGame_Manager final : public CBase
{
	DECLARE_SINGLETON(CGame_Manager)

public:
	enum EVENT {EVENT_ITEM,EVENT_GHOST, EVENT_END};

private:
	CGame_Manager();
	virtual ~CGame_Manager() = default;

public:
	void Tick(_float fTimeDelta);

	HRESULT Add_EventObject(_int iIndex, class CGameObject* pObject);
	void Broadcast_Message(_int iIndex, const _tchar* pMessage);

public:
	void Clear_Layer(_int iIndex);
	void Clear_List();


private:
	list<class CGameObject*>			m_ObjectList[EVENT_END];
		


public:
	virtual void Free() override;
};

END