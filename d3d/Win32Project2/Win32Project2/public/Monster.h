#pragma once

#include "Player.h"


class CMonster
{
public:
	CMonster();
	CMonster(CPlayer* pPlayer);
	~CMonster();
public:
	void	Initialize();
	void	Tick();
	void	LateTick();

	void	Render();
	void	Release();

private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	CPlayer*				m_pPlayer;

	IDirect3DVertexBuffer9*		m_bVertex;
	IDirect3DIndexBuffer9*		m_bIndex;

	D3DXMATRIX				m_matWorld;
	D3DXMATRIX				m_TargetWorld;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vRight;
	D3DXVECTOR3				m_vLook;
	D3DXVECTOR3				m_vUp;
};

