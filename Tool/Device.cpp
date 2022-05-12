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

//	HRESULT : 성공하게 되면 양수 값을 반환, 실패하게 되면 음수를 반환 시키도록 설계할 예정
HRESULT CDevice::Initialize(void)
{
	// 장치 초기화 과정
	/*  1. 하드웨어를 조사할 장치 객체 생성
	2. 하드웨어 조사(지원 수준)
	3. 하드웨어를 제어할 객체 생성*/

	 // 1. LPDIRECT3D9 객체 생성

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 2. 하드웨어 조사
	
	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보(지원 수준)를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// afx가 이름이 붙은 함수는 MFC에서 제공하는 함수
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp = 0;		// 버텍스 프로세싱 = 정점의 변환 + 조명연산

	// HAL을 통해 조사한 장치가 하드웨어 버텍스 프로세싱을 지원한다면
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	// 3. LPDIRECT3DDEVICE9 객체 생성

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Partameters(d3dpp);

	// CreateDevice : 장치를 제어할 그래픽 컴객체를 생성
	/*
	1. 어떤 그래픽 카드를 제어할 것인가
	2. 어떤 정보로 장치에 접근할 것인가
	3. 장치를 사용할 윈도우 핸들
	4. 동장 방식
	5. 사용환경
	6. 생성할 컴객체 주소*/
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 컴 객체 

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	// 폰트 컴 객체

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Width = 15;
	tFontInfo.Height = 15;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");


	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Create Font Failed");
		return E_FAIL;
	}

		
	//return E_FAIL;
	return S_OK;
}

// 후면 버퍼
void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,			// 지우고자 하는 렉트의 개수
					nullptr,	// 렉트 배열의 첫 번째 주소	, nullptr인 경우 전체 영역을 비우겠다는 의미
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	// 스텐실, 깊이, 타겟 버퍼 비우겠다는 의미
					D3DCOLOR_ARGB(255, 125, 125, 125),	// 백 버퍼의 색상
					1.f,		// 깊이 버퍼의 초기화 값(매우 중요!!!!!!!!!!!!!)
					0);			// 스텐실 버퍼의 초기화 값

	
	// 여기서부터 후면 버퍼에 그리기를 시작하겠다.
	m_pDevice->BeginScene();


	// 2D이미지를 그릴 수 있도록 장치를 준비(렌더링 옵션) // 알파 테스트가 유효한 상태에서 알파 블랜딩 사용하겠다는 옵션
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}




// 스왑체인 직전의 후면 버퍼
void CDevice::Render_End(HWND hWnd)
{


	m_pSprite->End();
	m_pDevice->EndScene();

	// 버퍼 스왑을 진행
	// 1, 2인자 : 스왑 체인이 D3DSWAPEFFECT_COPY로 작성하지 않는한 NULL
	// 3인자 : 출력 대상 윈도우 핸들
	// 4인자 :  스왑 체인이 D3DSWAPEFFECT_COPY로 작성하지 않는한 NULL

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

	
	/*D3DSWAPEFFECT_DISCARD = 스왑체인 방식
	D3DSWAPEFFECT_FLIP = 버퍼 하나를 뒤집으면서 사용하는 방식
	D3DSWAPEFFECT_COPY = 더블 버퍼링과 유사한 복사 방식*/
	
	//스왑 체인 : 페이지 플리핑 기법으로 이뤄져 있으며 전면 버퍼는 오로지 출력, 후면 버퍼는 지우고 다음 화면 출력할 것을 그리는 역할 분담으로 서로 가리키는 대상을 교체하여 깜빡거림 방지 및 화면 갱신을 수월하게 진행할 수 있다.

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 스왑체인 방식

	d3dpp.hDeviceWindow = g_hWnd;
	
	//창 모드로 실행, 전체화면 실행
	d3dpp.Windowed =  TRUE;		// TRUE인 경우 창 모드 설정

	// 타겟 버퍼(그리기 버퍼), 스텐실 버퍼(반사가 되는 물질 표현), 깊이 버퍼(깊이 값을 저장)
	// 자동으로 스텐실 버퍼와 깊이버퍼를 관리하겠다는 옵션
	d3dpp.EnableAutoDepthStencil = TRUE;
	//깊이 버퍼 24비트, 스텐실 버퍼 8비트
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	// 전체화면 시에만 사용되는 매개 변수들
	// 창 모드에서는 운영체제가 재생률을 관리함
	// 전체화면 시에는 그래픽 카드가 개입하게 된다.

	// 전체화면 시 현재 모니터 재생률과의 관계(현재 모니터 재생율을 토대로 알아서 수행)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 재생률과 시연 간격설정
	// D3DPRESENT_INTERVAL_IMMEDIATE  : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 장치가 알아서 결정(보통 모니터 재생율을 따라감)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
