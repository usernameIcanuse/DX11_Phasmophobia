#pragma once

/* 객체들의 원형을 관리하낟. */
/* 원형객체를 찾아서 복제하여 씬(레벨)별로 모아서 관린하다.*/

#include "Base.h"

BEGIN(Engine)


class CEvent_Manager final : public CBase
{
	DECLARE_SINGLETON(CEvent_Manager)

private:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;

public:
	
public:
	void Clear_Desc();
	STAGEDESC	Get_StageDesc()
	{
		return m_StageDesc;
	}


private:
	STAGEDESC		m_StageDesc;

public:
	virtual void Free() override;
};

END