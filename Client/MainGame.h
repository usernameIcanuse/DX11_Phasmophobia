#pragma once

#include "Include.h"
#include "TextureMgr.h"

class CDevice;
class CMainGame
{
public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(void);
	void		Release(void);

private:
	CDevice*			m_pGraphicDev = nullptr;

	TCHAR				m_szFPS[MIN_STR];
	int					m_iFps;

public:
	CMainGame();
	~CMainGame();
};

