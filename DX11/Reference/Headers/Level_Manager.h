#pragma once

#include "Base.h"

/* ���� ���ӳ��� ��������� ������ �����Ѵ�. */
/* ������ü�ÿ� ���� ������ �����Ѵ�. */
/* ������������ ����ߴ� ��ü�� �ڿ�(���ҽ���)�� �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);
	void Tick(_float fTimeDelta);
	HRESULT Render();

	void	Set_Current_Level(_uint _iCurrentLevelID)
	{
		m_iCurrentLevelID = _iCurrentLevelID;
	}
	_uint	Get_Current_Level()
	{
		return m_iCurrentLevelID;
	}

private:
	class CLevel*			m_pCurrentLevel = nullptr;
	_uint					m_iCurrentLevelID = 0;

public:
	virtual void Free() override;
};

END