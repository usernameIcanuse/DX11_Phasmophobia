#pragma once

#include "Base.h"

/* 현재 게임내에 보여줘야할 레벨을 보관한다. */
/* 레벨교체시에 기존 레벨을 삭제한다. */
/* 기존레벨에서 사용했던 객체와 자원(리소스등)을 정리한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel, _uint iNextLevelID);
	void Tick(_float fTimeDelta);
	HRESULT Render();

	void	Set_Current_Level(_uint _iCurrentLevelID)
	{
		m_iCurrentLevelID = _iCurrentLevelID;
	}
	_uint	Get_Current_Level()
	{
		return m_iCurrentLevelID;//loading 레벨에서 호출하기 때문에
	}
	_uint Get_Next_Level()
	{
		return m_iNextLevel;
	}

private:
	class CLevel*			m_pCurrentLevel = nullptr;
	_uint					m_iCurrentLevelID = 0;

	_uint					m_iNextLevel = 0;

public:
	virtual void Free() override;
};

END