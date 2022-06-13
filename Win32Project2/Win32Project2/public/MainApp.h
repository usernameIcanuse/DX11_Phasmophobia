#pragma once
#include "Define.h"

class CDevice;
class CPlayer;
class CMonster;
class CTerrain;

class CMainApp
{
	
public:
	CMainApp();
	~CMainApp();

public:
	HRESULT			Initialize();
	void			Tick();
	void			LateTick();
	void			Render();

	void			Release();

private:
	LPDIRECT3DDEVICE9		m_pDevice ;
	CDevice*			m_pGraphicDev ;

private:
	CPlayer*		m_pPlayer = nullptr;
	//CMonster*		m_pMonster = nullptr;
	CTerrain*		m_pTerrain = nullptr;

	ID3DXMesh*			m_pSphere;
	D3DXMATRIX			m_matSphereWorld;
	int					m_iTime = 0;

	bool				m_bChange = false;

	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DTEXTURE9	m_pTexture2;
	IDirect3DVertexBuffer9* m_pVB;

	D3DXMATRIX matBillboard;
};


