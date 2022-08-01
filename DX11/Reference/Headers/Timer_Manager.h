#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)
public:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);

private:
	map<const _tchar*, class CTimer*>			m_Timers;
	typedef map<const _tchar*, class CTimer*>	TIMERS;

private:
	class CTimer* Find_Timer(const _tchar* pTimerTag);

public:
	virtual void Free();
};

END