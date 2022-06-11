#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice);

CDevice::CDevice()
	:m_p3D(nullptr), m_pDevice(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::InitDevice(void)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);
	//IDerect3D9형태의 장치 검증과 장치 객체 생성 용도

	D3DCAPS9	caps;
	if (FAILED(m_p3D->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps
	)))
	{
		::MessageBox(0, L"GetDeviceCaps FAILED", 0, 0);
		return E_FAIL;
	}
	//하드웨어가 버텍스 프로세싱(조명연산,변환)을 시행할 수 있는지 확인

	DWORD vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//조사한 장치의 파라미터 설정
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);
	//다 조사한 뒤 조사한 정보를 바탕으로 인터페이스 객체 생성

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
		return E_FAIL;
	}


	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	//백버퍼 설정
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
////////////////////////////////////////////////////
	//멀티샘플링 설정. 현재는 사용하지 않음
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
////////////////////////////////////////////////////
	//버퍼 변환 설정. 현재는 스왑체인
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
////////////////////////////////////////////////////
	//전체화면 or 창모드 설정

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = TRUE;//창모드 FALSE전체화면
////////////////////////////////////////////////////
	//깊이,스텐실 포멧
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
////////////////////////////////////////////////////
	//추가 사용할 기능 유무
	d3dpp.Flags = 0;
////////////////////////////////////////////////////
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 144, 144, 144),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값

	m_pDevice->BeginScene();

}

void CDevice::Render_End(HWND hWnd)
{
	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);//후면 버퍼 시연(전면, 후면 버퍼 스왑)
}

void CDevice::Release()
{
	m_pDevice->Release();
	m_pDevice = nullptr;
	m_p3D->Release();
	m_p3D = nullptr;
	
	//IDirectx9을 해제할 때는 외부에서 delete가 아니라 해당 객체 안에 있는 Release를 불러야함

}
