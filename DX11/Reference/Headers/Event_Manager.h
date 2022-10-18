#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

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