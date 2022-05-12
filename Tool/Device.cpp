#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

//	HRESULT : �����ϰ� �Ǹ� ��� ���� ��ȯ, �����ϰ� �Ǹ� ������ ��ȯ ��Ű���� ������ ����
HRESULT CDevice::Initialize(void)
{
	// ��ġ �ʱ�ȭ ����
	/*  1. �ϵ��� ������ ��ġ ��ü ����
	2. �ϵ���� ����(���� ����)
	3. �ϵ��� ������ ��ü ����*/

	 // 1. LPDIRECT3D9 ��ü ����

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. �ϵ���� ����
	
	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ����(���� ����)�� ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// afx�� �̸��� ���� �Լ��� MFC���� �����ϴ� �Լ�
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp = 0;		// ���ؽ� ���μ��� = ������ ��ȯ + ������

	// HAL�� ���� ������ ��ġ�� �ϵ���� ���ؽ� ���μ����� �����Ѵٸ�
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	// 3. LPDIRECT3DDEVICE9 ��ü ����

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Partameters(d3dpp);

	// CreateDevice : ��ġ�� ������ �׷��� �İ�ü�� ����
	/*
	1. � �׷��� ī�带 ������ ���ΰ�
	2. � ������ ��ġ�� ������ ���ΰ�
	3. ��ġ�� ����� ������ �ڵ�
	4. ���� ���
	5. ���ȯ��
	6. ������ �İ�ü �ּ�*/
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// ��������Ʈ �� ��ü 

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// ��Ʈ �� ��ü

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Width = 15;
	tFontInfo.Height = 15;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");


	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Create Font Failed");
		return E_FAIL;
	}

		
	//return E_FAIL;
	return S_OK;
}

// �ĸ� ����
void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,			// ������� �ϴ� ��Ʈ�� ����
					nullptr,	// ��Ʈ �迭�� ù ��° �ּ�	, nullptr�� ��� ��ü ������ ���ڴٴ� �ǹ�
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	// ���ٽ�, ����, Ÿ�� ���� ���ڴٴ� �ǹ�
					D3DCOLOR_ARGB(255, 125, 125, 125),	// �� ������ ����
					1.f,		// ���� ������ �ʱ�ȭ ��(�ſ� �߿�!!!!!!!!!!!!!)
					0);			// ���ٽ� ������ �ʱ�ȭ ��

	
	// ���⼭���� �ĸ� ���ۿ� �׸��⸦ �����ϰڴ�.
	m_pDevice->BeginScene();


	// 2D�̹����� �׸� �� �ֵ��� ��ġ�� �غ�(������ �ɼ�) // ���� �׽�Ʈ�� ��ȿ�� ���¿��� ���� ���� ����ϰڴٴ� �ɼ�
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}




// ����ü�� ������ �ĸ� ����
void CDevice::Render_End(HWND hWnd)
{


	m_pSprite->End();
	m_pDevice->EndScene();

	// ���� ������ ����
	// 1, 2���� : ���� ü���� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL
	// 3���� : ��� ��� ������ �ڵ�
	// 4���� :  ���� ü���� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL

	m_pDevice->Present(NULL, NULL, hWnd, NULL);

}

void CDevice::Release(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Partameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth	= WINCX;
	d3dpp.BackBufferHeight	= WINCY;
	d3dpp.BackBufferFormat  = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	
	/*D3DSWAPEFFECT_DISCARD = ����ü�� ���
	D3DSWAPEFFECT_FLIP = ���� �ϳ��� �������鼭 ����ϴ� ���
	D3DSWAPEFFECT_COPY = ���� ���۸��� ������ ���� ���*/
	
	//���� ü�� : ������ �ø��� ������� �̷��� ������ ���� ���۴� ������ ���, �ĸ� ���۴� ����� ���� ȭ�� ����� ���� �׸��� ���� �д����� ���� ����Ű�� ����� ��ü�Ͽ� �����Ÿ� ���� �� ȭ�� ������ �����ϰ� ������ �� �ִ�.

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// ����ü�� ���

	d3dpp.hDeviceWindow = g_hWnd;
	
	//â ���� ����, ��üȭ�� ����
	d3dpp.Windowed =  TRUE;		// TRUE�� ��� â ��� ����

	// Ÿ�� ����(�׸��� ����), ���ٽ� ����(�ݻ簡 �Ǵ� ���� ǥ��), ���� ����(���� ���� ����)
	// �ڵ����� ���ٽ� ���ۿ� ���̹��۸� �����ϰڴٴ� �ɼ�
	d3dpp.EnableAutoDepthStencil = TRUE;
	//���� ���� 24��Ʈ, ���ٽ� ���� 8��Ʈ
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	// ��üȭ�� �ÿ��� ���Ǵ� �Ű� ������
	// â ��忡���� �ü���� ������� ������
	// ��üȭ�� �ÿ��� �׷��� ī�尡 �����ϰ� �ȴ�.

	// ��üȭ�� �� ���� ����� ��������� ����(���� ����� ������� ���� �˾Ƽ� ����)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// ������� �ÿ� ���ݼ���
	// D3DPRESENT_INTERVAL_IMMEDIATE  : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ ��ġ�� �˾Ƽ� ����(���� ����� ������� ����)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
