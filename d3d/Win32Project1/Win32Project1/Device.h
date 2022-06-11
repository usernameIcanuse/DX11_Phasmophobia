#pragma once
#include "Define.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

public:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pDevice; }

public:
	HRESULT					InitDevice(void);
	void					SetParameters(D3DPRESENT_PARAMETERS& d3dpp);

	void					Render_Begin(void);
	void					Render_End(HWND hWnd = NULL);
		
	void					Release();
private:
	LPDIRECT3D9				m_p3D;
	LPDIRECT3DDEVICE9		m_pDevice;
};

