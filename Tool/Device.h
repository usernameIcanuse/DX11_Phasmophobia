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

	// ��ġ�ʱ�ȭ ����
/*  1. �ϵ��� ������ ��ġ ��ü ����
	2. �ϵ���� ����(���� ����)
	3. �ϵ��� ������ ��ü ����*/

	// COM(COMPONENT object model) ��ü : ���� ȯ�濡 ������� ȣȯ�� ������ ������Ʈ�� ����� ���� ms���� ������ ǥ��ȭ�� ���

private:
	void		Set_Partameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// ����ϰ��� �ϴ� �ϵ���� ��ġ�� �����ϰ�, �ش� ��ġ�� �����ϱ� ���� ��ü ������ ���ִ� ������
	LPDIRECT3DDEVICE9	m_pDevice;	// �׷��� ��ġ�� ���� �������� �����ϴ� ��ü ������
	LPD3DXSPRITE		m_pSprite;	// dx�󿡼� 2D �̹����� ������ִ� com ��ü ������
	LPD3DXFONT			m_pFont;	// dx�󿡼� ��Ʈ ����� ����ϴ� ��ü ������
};

