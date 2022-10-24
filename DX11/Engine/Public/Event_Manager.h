#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

#include "Base.h"

BEGIN(Engine)


class CEvent_Manager final : public CBase
{
	DECLARE_SINGLETON(CEvent_Manager)

public:
	enum STAGE_TYPE { PLAYTIME, HOUSETIME,AROUNDGHOST,EVENT,INTERACTION};
private:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;

public:
	void Add_Desc(_uint eType, _float fValue);
	void Set_GhostName(const _tchar* _szGhostName);
public:
	void Clear_Desc();

	STAGEDESC*	Get_StageDesc()
	{
		return &m_StageDesc;
	}


private:
	STAGEDESC		m_StageDesc;
	_tchar			m_szGhostName[10];

public:
	virtual void Free() override;
};

END