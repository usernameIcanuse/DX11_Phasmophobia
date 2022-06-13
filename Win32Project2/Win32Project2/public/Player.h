#pragma once

#include "Struct.h"

class CPlayer
{
public:
	CPlayer();
	~CPlayer();
public:
	void	Initialize();
	void	Tick();
	void	LateTick();
	
	void	Render();
	void	Release();
	
private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

	IDirect3DVertexBuffer9*		m_bVertex;


	IDirect3DIndexBuffer9*		m_bIndex;

	D3DXMATRIX				m_matWorld;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vLook;
	D3DXVECTOR3				m_vUp;

	float LastX;
	float LastY;

	float m_fJumpAccel = -0.05f;
	float m_fJumpSpeed = 0.f;
	bool	m_bJump = false;
	
};

