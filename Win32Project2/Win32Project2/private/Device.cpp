#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	:m_p3D(nullptr), m_pDevice(nullptr)
{

}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::InitDevice()
{
	
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;
	if (FAILED(m_p3D->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps)))
	{
		::MessageBox(0, L"GetDeviceCaps FAILED", 0, 0);
		return E_FAIL;
	}

	DWORD vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS  d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,g_hWnd,vp,&d3dpp,&m_pDevice)))
	{
		::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS & d3dpp)
{

	d3dpp.BackBufferWidth = 1280;
	d3dpp.BackBufferHeight = 720;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = TRUE;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 200, 200, 200),
		1.f, 0);
	m_pDevice->BeginScene();

}

void CDevice::Render_End(HWND hwnd)
{
	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, hwnd, nullptr);
}

void CDevice::Release()
{
	m_pDevice->Release();
	m_pDevice = nullptr;
	m_p3D->Release();
	m_p3D = nullptr;

}
