#pragma once

#include "Define.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)
	
public:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9	 Get_Device() { return m_pDevice; }

public:
	HRESULT				 InitDevice();
	void				 SetParameters(D3DPRESENT_PARAMETERS& d3dpp);
	
	void				 Render_Begin();
	void				 Render_End(HWND hwnd = NULL);

	void				 Release();

private:
	LPDIRECT3D9				m_p3D;
	LPDIRECT3DDEVICE9		m_pDevice;
};

