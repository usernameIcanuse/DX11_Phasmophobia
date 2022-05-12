#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT				Get_Font(void) { return m_pFont;  }

public:
	HRESULT			Initialize(void);
	void			Render_Begin(void);
	void			Render_End(HWND hWnd = NULL);
	void			Release(void);

	// 장치초기화 과정
/*  1. 하드웨어를 조사할 장치 객체 생성
	2. 하드웨어 조사(지원 수준)
	3. 하드웨어를 제어할 객체 생성*/

	// COM(COMPONENT object model) 객체 : 개발 환경에 상관없이 호환이 가능한 컴포넌트의 사용을 위해 ms에서 규정한 표준화된 방법

private:
	void		Set_Partameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// 사용하고자 하는 하드웨어 장치를 조사하고, 해당 장치를 제어하기 위한 객체 생성을 해주는 포인터
	LPDIRECT3DDEVICE9	m_pDevice;	// 그래픽 장치를 통한 렌더링을 제어하는 객체 포인터
	LPD3DXSPRITE		m_pSprite;	// dx상에서 2D 이미지를 출력해주는 com 객체 포인터
	LPD3DXFONT			m_pFont;	// dx상에서 폰트 출력을 담당하는 객체 포인터
};

