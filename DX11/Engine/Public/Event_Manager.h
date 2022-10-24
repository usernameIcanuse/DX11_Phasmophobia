#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

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