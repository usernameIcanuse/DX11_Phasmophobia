#pragma once
#include "Define.h"

class CCamera
{
	DECLARE_SINGLETON(CCamera)
	
public:
	CCamera();
	~CCamera();

public:
	void	TransForm(D3DXMATRIX&  World);
	void	TransView();
	D3DXMATRIX	GetCameraMatrix();
	D3DXVECTOR3 GetCameraPos() {	return vPlayerpos;}
	void PlayerPos(D3DXVECTOR3& vPos) { vPlayerpos = vPos; }

	void	Initialize();

private:
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	D3DXVECTOR3  vPos;
	D3DXVECTOR3  vUp;
	D3DXVECTOR3  vLook;
	D3DXVECTOR3  vRight;
	D3DXVECTOR3  vPlayerpos;
	D3DXVECTOR3  vCenter;
	int			m_iCnt = 0;;

	bool		m_bRButton = false;
	bool		m_bCtrl = false;
	D3DXMATRIX	View;
	
	float LastX;
	float LastY;
	
	float xoffset=0.f;
	float yoffset=0.f;

	D3DXVECTOR3		vEye;
};	

