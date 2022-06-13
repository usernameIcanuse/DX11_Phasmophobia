#pragma once
#include "Define.h"

class CZFrustum
{
	DECLARE_SINGLETON(CZFrustum)

public:
	CZFrustum();
	~CZFrustum();

public:
	bool Make(D3DXMATRIX* pmatViewProj);//투영 행렬을 받아와서 절두체 생성
	bool IsIn(D3DXVECTOR3* vp);//점 판정
	bool IsInsPhere(D3DXVECTOR3* vp, float radius);//경계구 판정
	void Draw();

	void Release();
	void LateTick();
	

private:
	LPDIRECT3DDEVICE9 m_pDevice;
private:
	D3DXVECTOR3 m_vtx[8];//절두체 정점
	D3DXVECTOR3 m_vPos;//현재 카메라 좌표
	D3DXPLANE m_pPlane[6];//절두체 각 평면


};

